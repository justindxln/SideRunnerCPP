// Copyright Epic Games, Inc. All Rights Reserved.

#include "SideRunnerCPPGameMode.h"
#include "SideRunnerCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Spikes.h"
#include "KillWall.h"
#include "RunnerCharacter.h"
#include "SideRunnerPlayerController.h"
#include "SideRunnerSaveGame.h"

ASideRunnerCPPGameMode::ASideRunnerCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

const float ASideRunnerCPPGameMode::DistanceMax = 2000.f;
const float ASideRunnerCPPGameMode::DistanceMin = 800.f;
const float ASideRunnerCPPGameMode::AnimSpeedMax = 2.5f;
const float ASideRunnerCPPGameMode::AnimSpeedMin = 0.3f;

void ASideRunnerCPPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	// Clear current menu widget
	if (CurrentMenuWidget != nullptr)
	{
		CurrentMenuWidget->RemoveFromViewport();
		CurrentMenuWidget = nullptr;
		//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("CLEARING OUT OLD WIDGET"));
	}

	// Create new menu widget and add to Viewport
	if (NewWidgetClass != nullptr)
	{
		CurrentMenuWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentMenuWidget != nullptr)
		{
			CurrentMenuWidget->AddToViewport();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ADDED NEW WIDGET"));
		}
	}
}

void ASideRunnerCPPGameMode::ChangeHUDWidget(TSubclassOf<UUserWidget> NewHUDClass)
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

	// Set up kill wall
	FActorSpawnParameters WallInfo = FActorSpawnParameters();
	KillWall = GetWorld()->SpawnActor<AKillWall>(KillWallClass, WallLocation, WallRotation, WallInfo);
	KillWall->SetMoveSpeed(WallMoveSpeed);

	// Get controller reference, load settings, open main menu widget
	PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	LoadGame();
	ChangeMenuWidget(StartingWidgetClass);
}

void ASideRunnerCPPGameMode::StartNewGame()
{
	// Set up player character
	if (!PlayerCharacter) {
		PlayerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	PlayerCharacter->ToggleMovement();
	PlayerCharacter->SetupMovementProperties(CharacterRunSpeed, CharacterDoubleJumpAllowed, CharacterDoubleJumpCoolDown);

	// Set up Player Status Manager
	if (PlayerStatusManager != nullptr) {
		Destroy(PlayerStatusManager);
	}
	PlayerStatusManager = GetWorld()->SpawnActor<APlayerStatusManager>(PlayerStatusManagerClass, FActorSpawnParameters());

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
}

// Get light color for the HUD glowing light depending on distance between player and wall
FLinearColor ASideRunnerCPPGameMode::GetDistanceLightColor()
{
	if (!PlayerCharacter || !KillWall) { return FLinearColor::White; }

	FLinearColor NearColor = FLinearColor::Red; 
	FLinearColor MidColor = FLinearColor::Yellow;
	FLinearColor FarColor = FLinearColor::Green;

	float WallDistance = GetPlayerWallDistance();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("distance is %f"), WallDistance));

	if (WallDistance > DistanceMax) { return FarColor; }

	float ColorRatio = (FMath::Clamp(WallDistance, DistanceMin, DistanceMax) - DistanceMin) / (DistanceMax - DistanceMin);

	return FLinearColor::LerpUsingHSV(NearColor, MidColor, ColorRatio);
}


// Get the speed multiplier for the HUD glowing light depending on distance between player and wall
float ASideRunnerCPPGameMode::GetGlowAnimSpeed()
{
	float SpeedRatio = 1.0f - (FMath::Clamp(GetPlayerWallDistance(), DistanceMin, DistanceMax) - DistanceMin) / (DistanceMax - DistanceMin);

	return FMath::Lerp(SpeedRatio, AnimSpeedMax, AnimSpeedMin);
}

void ASideRunnerCPPGameMode::SaveGame()
{
	if (USideRunnerSaveGame* SaveGameInstance = Cast<USideRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(USideRunnerSaveGame::StaticClass())))
	{
		// Set current data on savegame object
		SaveGameInstance->PlayerSpeed = CharacterRunSpeed;
		SaveGameInstance->WallSpeed = WallMoveSpeed;
		SaveGameInstance->PlayerDoubleJump = CharacterDoubleJumpAllowed;
		SaveGameInstance->HighScores = this->HighScoreArray;
		SaveGameInstance->PlayerDoubleJumpCoolDown = CharacterDoubleJumpCoolDown;
		SaveGameInstance->PlayerNames = PlayerNameArray;

		// Start ASync Save
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, FString(TEXT("DEFAULT")), 0);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString((TEXT("SAVING DATA"))));
	}
}

void ASideRunnerCPPGameMode::LoadGame()
{
	if (USideRunnerSaveGame* SaveGameInstance = Cast<USideRunnerSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("DEFAULT")), 0)))
	{
		// Load save data into settings
		CharacterRunSpeed = SaveGameInstance->PlayerSpeed;
		WallMoveSpeed = SaveGameInstance->WallSpeed;
		CharacterDoubleJumpAllowed = SaveGameInstance->PlayerDoubleJump;
		this->HighScoreArray = SaveGameInstance->HighScores;
		CharacterDoubleJumpCoolDown = SaveGameInstance->PlayerDoubleJumpCoolDown;
		PlayerNameArray = SaveGameInstance->PlayerNames;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString((TEXT("LOADING DATA"))));
	}
}

void ASideRunnerCPPGameMode::TriggerDeath()
{
	// Stop the wall from moving and save new high score
	KillWall->SetCanMove(false);
	int32 ScoreCurrent = FMath::RoundToInt(GetPlayerWallDistance());

	// Insert new score among current high score array if it's high enough, then trim the array
	for (int32 i = 0; i < HighScoreArray.Num(); i++) {
		if (ScoreCurrent > HighScoreArray[i]) {
			HighScoreArray.Insert(ScoreCurrent, i);
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
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASideRunnerCPPGameMode::ShowGameOverMenu, 2.f, false);
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

float ASideRunnerCPPGameMode::GetPlayerWallDistance()
{
	return FVector::Dist(PlayerCharacter->GetActorLocation(), KillWall->GetActorLocation());
}

void ASideRunnerCPPGameMode::SetHighScoreText()
{
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

void ASideRunnerCPPGameMode::SetPlayerName(FText PlayerName)
{
	PlayerNameCurrent = PlayerName;
}