// Copyright Epic Games, Inc. All Rights Reserved.

#include "SideRunnerCPPGameMode.h"
#include "SideRunnerCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Spikes.h"
#include "KillWall.h"
#include "RunnerCharacter.h"
#include "SideRunnerPlayerController.h"
#include "SideRunnerSaveGame.h"
#include "PlayerStatusManager.h"
#include "LevelGenerator.h"

ASideRunnerCPPGameMode::ASideRunnerCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ASideRunnerCPPGameMode::Tick(float DeltaTime)
{
	// Only increase the score when the game is active
	if (GameState == EGameState::Active) {
	CurrentScoreMultiplier = GetScoreMultiplier();
	CurrentScore += (CurrentScoreMultiplier * 10.f) * DeltaTime;
	}
}

void ASideRunnerCPPGameMode::ChangeMenuWidget(const TSubclassOf<UUserWidget> NewWidgetClass)
{
	// Clear current menu widget
	if (CurrentMenuWidget != nullptr)
	{
		CurrentMenuWidget->RemoveFromViewport();
		CurrentMenuWidget = nullptr;
	}

	// Create new menu widget and add to Viewport
	if (NewWidgetClass != nullptr)
	{
		CurrentMenuWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentMenuWidget != nullptr)
		{
			CurrentMenuWidget->AddToViewport();
		}
	}
}

void ASideRunnerCPPGameMode::ChangeHUDWidget(const TSubclassOf<UUserWidget> NewHUDClass)
{
	// Clear current HUD
	if (CurrentHUDWidget != nullptr)
	{
		CurrentHUDWidget->RemoveFromViewport();
		CurrentHUDWidget = nullptr;
	}

	// Create new HUD and add to Viewport
	if (NewHUDClass != nullptr)
	{
		CurrentHUDWidget = CreateWidget<UUserWidget>(GetWorld(), NewHUDClass);
		if (CurrentHUDWidget != nullptr) {
			CurrentHUDWidget->AddToViewport();
			//CurrentHUDWidget.PlayAnimation 
		}
	}
}

void ASideRunnerCPPGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameState = EGameState::PreGame;

	// Set up kill wall
	const FActorSpawnParameters WallInfo = FActorSpawnParameters();
	KillWall = GetWorld()->SpawnActor<AKillWall>(KillWallClass, WallLocation, WallRotation, WallInfo);
	KillWall->SetMoveSpeed(WallMoveSpeed);

	// Get controller reference, load settings, open main menu widget
	PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	LoadGame();
	ChangeMenuWidget(StartingWidgetClass);
}

void ASideRunnerCPPGameMode::StartNewGame()
{
	GameState = EGameState::Active;

	// Set up player character
	if (!PlayerCharacter) {
		PlayerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	PlayerCharacter->ToggleMovement();
	PlayerCharacter->SetupMovementProperties(CharacterRunSpeed, bCharacterDoubleJumpAllowed, CharacterDoubleJumpCoolDown);
	CurrentScore = 0.f;

	// Set up Player Status Manager
	if (PlayerStatusManager != nullptr) {
		Destroy(PlayerStatusManager);
	}
	PlayerStatusManager = GetWorld()->SpawnActor<APlayerStatusManager>(PlayerStatusManagerClass, FActorSpawnParameters());
	PlayerStatusManager->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	// Set up Level Generator
	if (LevelGenerator != nullptr) {
		Destroy(LevelGenerator);
	}
	LevelGenerator = GetWorld()->SpawnActor<ALevelGenerator>(LevelGeneratorClass, FActorSpawnParameters());
	LevelGenerator->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	// Set input mode in player controller
	if (!PlayerController) {
		PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}
	PlayerController->SetInputGameOnly();

	// Get the wall moving
	if (KillWall) KillWall->SetCanMove();

	// Clear menu widgets
	ChangeMenuWidget(nullptr);

	// Add HUD widget to viewport
	ChangeHUDWidget(HUDWidgetClass);

	// Set up references for Player Status Manager
	if (PlayerStatusManager) PlayerStatusManager->SetClassReferences(this, CurrentHUDWidget);
}

// Get Alpha/Ratio value for Lerping in HUD and Scoring functions
float ASideRunnerCPPGameMode::GetWallDistanceLerpRatio(const bool bClampUpper/*= true*/) const
{
	const float WallDistance = FMath::Clamp(GetPlayerWallDistance(), DistanceMin, bClampUpper ? DistanceMax : GetPlayerWallDistance());

	return (WallDistance - DistanceMin) / (DistanceMax - DistanceMin);
}

void ASideRunnerCPPGameMode::SaveGame() const
{
	if (USideRunnerSaveGame* const SaveGameInstance = Cast<USideRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(USideRunnerSaveGame::StaticClass())))
	{
		// Set current data on savegame object
		SaveGameInstance->PlayerSpeed = CharacterRunSpeed;
		SaveGameInstance->WallSpeed = WallMoveSpeed;
		SaveGameInstance->bPlayerDoubleJump = bCharacterDoubleJumpAllowed;
		SaveGameInstance->HighScores = HighScoreArray;
		SaveGameInstance->PlayerDoubleJumpCoolDown = CharacterDoubleJumpCoolDown;
		SaveGameInstance->PlayerNames = PlayerNameArray;

		// Start ASync Save
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, FString(TEXT("DEFAULT")), 0);
	}
}

void ASideRunnerCPPGameMode::LoadGame()
{
	if (USideRunnerSaveGame* SaveGameInstance = Cast<USideRunnerSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("DEFAULT")), 0)))
	{
		// Load save data into settings
		CharacterRunSpeed = SaveGameInstance->PlayerSpeed;
		WallMoveSpeed = SaveGameInstance->WallSpeed;
		bCharacterDoubleJumpAllowed = SaveGameInstance->bPlayerDoubleJump;
		HighScoreArray = SaveGameInstance->HighScores;
		CharacterDoubleJumpCoolDown = SaveGameInstance->PlayerDoubleJumpCoolDown;
		PlayerNameArray = SaveGameInstance->PlayerNames;
	}
}

FText ASideRunnerCPPGameMode::GetScoreBoostText() const
{
	return FText::FromString(FString::Printf(TEXT("x%i"), CurrentScoreBoost));
}

void ASideRunnerCPPGameMode::TriggerScoreBoost()
{
	KillWall->BoostMoveSpeed(100.f);
	ScoreMultiplierMax += 1.0f;
	CurrentScoreBoost++;
}

void ASideRunnerCPPGameMode::TriggerDeath()
{
	DoOnTriggerDeath();
	GameState = EGameState::PostGame;

	// Stop the wall from moving and save new high score
	KillWall->SetCanMove(false);
	const int32 CurrentScoreInt = FMath::RoundToInt(CurrentScore);

	// Insert new score among current high score array if it's high enough, then trim the array
	for (int8 i = 0; i < HighScoreArray.Num(); i++) {
		if (CurrentScoreInt > HighScoreArray[i]) {
			HighScoreArray.Insert(FMath::RoundToInt(CurrentScoreInt), i);
			PlayerNameArray.Insert(PlayerNameCurrent, i);
			break;
		}
	}
	HighScoreArray.SetNum(MaxHighScores);
	PlayerNameArray.SetNum(MaxHighScores);

	SaveGame();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Score: %i\nHigh Score: %i"), ScoreCurrent, HighScore));

	SetHighScoreText();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASideRunnerCPPGameMode::ShowGameOverMenu, 1.f, false);
}

// Clear HUD and show Game Over Menu
void ASideRunnerCPPGameMode::ShowGameOverMenu()
{
	ChangeHUDWidget(nullptr);
	ChangeMenuWidget(GameOverWidgetClass);
}

void ASideRunnerCPPGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()));
}

float ASideRunnerCPPGameMode::GetPlayerWallDistance() const
{
	return FVector::Dist(PlayerCharacter->GetActorLocation(), KillWall->GetActorLocation());
}

float ASideRunnerCPPGameMode::GetScoreMultiplier() const
{
	// Get position on the multiplier scale based on distance
	// The closer the player to the wall, the higher the multiplier
	float OutMultiplier = FMath::Lerp(ScoreMultiplierMax, ScoreMultiplierMin, GetWallDistanceLerpRatio());

	// Round the multiplier to the nearest 0.5
	OutMultiplier = FMath::RoundToInt (OutMultiplier * 2) / 2.f;

	return OutMultiplier;
}

void ASideRunnerCPPGameMode::SetHighScoreText()
{
	// Set current score text
	FinalScoreText = FText::FromString(FString::Printf(TEXT("Your Score: %i"), FMath::RoundToInt(CurrentScore)));

	// Concat all scores in array to string then convert to text
	FString HighScoreString;
	for (auto& Score : HighScoreArray)
	{
		HighScoreString += FString::Printf(TEXT("%i\n"), Score);
	}	
	HighScoreText = FText::FromString(HighScoreString);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, HighScoreText.ToString());
	
	// Concat all names in array to string then convert to text
	FString NamesString;
	for (auto& Name : PlayerNameArray)
	{
		if (Name.IsEmpty()) Name = FText::FromString(TEXT("Unnamed Player"));
		NamesString += Name.ToString();
		NamesString += "\n";
	}
	PlayerNamesText = FText::FromString(NamesString);
}

void ASideRunnerCPPGameMode::SetPlayerName(const FText PlayerName)
{
	PlayerNameCurrent = PlayerName;

	if (PlayerName.ToString() == TEXT("password")) bIsTestMode = true;
}
