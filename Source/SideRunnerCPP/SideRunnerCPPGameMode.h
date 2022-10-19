// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SideRunnerCPPGameMode.generated.h"

class ASpikes;
class AKillWall;
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
	UFUNCTION(BlueprintCallable, Category = "Side Runner UI")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "Side Runner UI")
	void ChangeHUDWidget(TSubclassOf<UUserWidget> NewHUDClass);

	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "Side Runner HUD")
	FLinearColor GetDistanceLightColor();

	UFUNCTION(BlueprintCallable, Category = "Side Runner HUD")
	float GetGlowAnimSpeed();

	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void SaveGame();

	void LoadGame();

	void RestartGame();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner UI")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner Game")
	TSubclassOf<ASpikes> KillWallBP;

	UPROPERTY()
	UUserWidget* CurrentMenuWidget;

	UPROPERTY()
	UUserWidget* CurrentHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kill Wall Properties")
	float WallMoveSpeed = 350.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	float CharacterRunSpeed = 600.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	bool CharacterDoubleJumpAllowed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	float CharacterDoubleJumpCoolDown = 4.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Side Runner Game")
	float HighScore;

	ARunnerCharacter* PlayerCharacter;

	AKillWall* KillWall;

	ASideRunnerPlayerController* PlayerController;

private:
	static const float DistanceMin;
	static const float DistanceMax;
	static const float AnimSpeedMax;
	static const float AnimSpeedMin;
};



