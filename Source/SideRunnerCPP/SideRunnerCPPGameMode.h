// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SideRunnerCPPGameMode.generated.h"

class ASpikes;
class ARunnerCharacter;
class ASideRunnerPlayerController;

UCLASS(minimalapi)
class ASideRunnerCPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASideRunnerCPPGameMode();

public:
	// Remove Current Menu Widget and create new one if provided
	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void StartNewGame();

	void RestartGame();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner Game")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner Game")
	TSubclassOf<ASpikes> KillWall;

	UPROPERTY()
	UUserWidget* CurrentMenuWidget;

	UPROPERTY()
	UUserWidget* CurrentHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	float CharacterRunSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	bool CharacterDoubleJumpAllowed = false;

	ARunnerCharacter* PlayerCharacter;

	ASideRunnerPlayerController* PlayerController;
};



