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

void ASideRunnerCPPGameMode::StartNewGame()
{
	// Clear menu widgets
	ChangeMenuWidget(nullptr);

	// Add HUD widget to viewport
	ChangeHUDWidget(HUDWidgetClass);

	// Set up kill wall
	FVector WallLocation = FVector(-30.f, -450.f, 450.f);
	FRotator WallRotation = FRotator(0.f, 90.f, 0.f);
	FActorSpawnParameters WallInfo = FActorSpawnParameters();
	KillWall = GetWorld()->SpawnActor<AKillWall>(KillWallBP, WallLocation, WallRotation, WallInfo);
	KillWall->SetMoveSpeed(WallMoveSpeed);

	// Set up player character
	if (!PlayerCharacter) {
		PlayerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	PlayerCharacter->ToggleMovement();
	PlayerCharacter->SetupMovementProperties(CharacterRunSpeed, CharacterDoubleJumpAllowed, CharacterDoubleJumpCoolDown);

	// Set input mode in player controller
	if (!PlayerController) {
		PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}
	PlayerController->SetInputGameOnly();
}

// Get light color for the HUD glowing light depending on distance between player and wall
FLinearColor ASideRunnerCPPGameMode::GetDistanceLightColor()
{
	if (!PlayerCharacter || !KillWall) { return FLinearColor::White; }

	FLinearColor NearColor = FLinearColor::Red; 
	FLinearColor MidColor = FLinearColor::Yellow;
	FLinearColor FarColor = FLinearColor::Green;

	float WallDistance = FVector::Dist(PlayerCharacter->GetActorLocation(), KillWall->GetActorLocation());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("distance is %f"), WallDistance));

	if (WallDistance > DistanceMax) { return FarColor; }

	float ColorRatio = (FMath::Clamp(WallDistance, DistanceMin, DistanceMax) - DistanceMin) / (DistanceMax - DistanceMin);

	return FLinearColor::LerpUsingHSV(NearColor, MidColor, ColorRatio);
}


// Get the speed multiplier for the HUD glowing light depending on distance between player and wall
float ASideRunnerCPPGameMode::GetGlowAnimSpeed()
{
	float WallDistance = FVector::Dist(PlayerCharacter->GetActorLocation(), KillWall->GetActorLocation());
	float SpeedRatio = 1.0f - (FMath::Clamp(WallDistance, DistanceMin, DistanceMax) - DistanceMin) / (DistanceMax - DistanceMin);

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
		SaveGameInstance->HighScore = this->HighScore;
		SaveGameInstance->PlayerDoubleJumpCoolDown = CharacterDoubleJumpCoolDown;

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
		this->HighScore = SaveGameInstance->HighScore;
		CharacterDoubleJumpCoolDown = SaveGameInstance->PlayerDoubleJumpCoolDown;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString((TEXT("LOADING DATA"))));
	}
}

void ASideRunnerCPPGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()));
}

void ASideRunnerCPPGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get player and controller references, load settings, open main menu widget
	PlayerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	LoadGame();
	ChangeMenuWidget(StartingWidgetClass);
}
