// Copyright Epic Games, Inc. All Rights Reserved.

#include "SideRunnerCPPGameMode.h"
#include "SideRunnerCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Spikes.h"
#include "RunnerCharacter.h"
#include "SideRunnerPlayerController.h"

ASideRunnerCPPGameMode::ASideRunnerCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASideRunnerCPPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTEMPTING TO CHANGE WIDGET"));
	if (CurrentMenuWidget != nullptr)
	{
		CurrentMenuWidget->RemoveFromViewport();
		CurrentMenuWidget = nullptr;
		//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("CLEARING OUT OLD WIDGET"));
	}

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

void ASideRunnerCPPGameMode::StartNewGame()
{
	// CLEAR ANY MENU WIDGETS JUST IN CASE
	if (CurrentMenuWidget != nullptr) {
		CurrentMenuWidget->RemoveFromViewport();
		CurrentMenuWidget = nullptr;
	}

	// SPAWN KILL WALL
	FVector WallLocation = FVector(-30.f, -450.f, 450.f);
	FRotator WallRotation = FRotator(0.f, 90.f, 0.f);
	FActorSpawnParameters WallInfo = FActorSpawnParameters();
	GetWorld()->SpawnActor<ASpikes>(KillWall, WallLocation, WallRotation, WallInfo);
	if (!PlayerCharacter) {
		PlayerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}

	// SETUP PLAYER CHARACTER
	PlayerCharacter->ToggleMovement();
	PlayerCharacter->SetupMovementProperties(CharacterRunSpeed, CharacterDoubleJumpAllowed);
	if (!PlayerController) {
		PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}
	PlayerController->SetInputGameOnly();

	if (HUDWidgetClass != nullptr) {
		CurrentHUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentHUDWidget != nullptr) {
			CurrentHUDWidget->AddToViewport();
		}
	}
}

void ASideRunnerCPPGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()));
}

void ASideRunnerCPPGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerController = Cast<ASideRunnerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ChangeMenuWidget(StartingWidgetClass);
}
