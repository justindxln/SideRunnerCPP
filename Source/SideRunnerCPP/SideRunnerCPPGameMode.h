// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStatusManager.h"
#include "EnumName.h"
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

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly)
	APlayerStatusManager* PlayerStatusManager;

	// Swapping in/out menus
	UFUNCTION(BlueprintCallable, Category = "Side Runner UI")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	// Swapping in/out menus
	UFUNCTION(BlueprintCallable, Category = "Side Runner UI")
	void ChangeHUDWidget(TSubclassOf<UUserWidget> NewHUDClass);

	// HUD functions
	UFUNCTION(BlueprintCallable, Category = "Side Runner HUD")
	FLinearColor GetDistanceLightColor();
	UFUNCTION(BlueprintCallable, Category = "Side Runner HUD")
	float GetGlowAnimSpeed();

	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void SaveGame();

	void LoadGame();

	void TriggerDeath();

protected:
	// Blueprint classes to spawn from
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner UI")
	TSubclassOf<UUserWidget> StartingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Side Runner Game")
	TSubclassOf<ASpikes> KillWallClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Manager Blueprints")
	TSubclassOf<APlayerStatusManager> PlayerStatusManagerClass;

	// Current Widget references
	UPROPERTY()
	UUserWidget* CurrentMenuWidget;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentHUDWidget;

	// Kill Wall properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kill Wall Properties")
	float WallMoveSpeed = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kill Wall Properties")
	FVector WallLocation = FVector(-30.f, -950.f, 520.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kill Wall Properties")
	FRotator WallRotation = FRotator(0.f, 90.f, 0.f);

	// Character movement properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	float CharacterRunSpeed = 600.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	bool CharacterDoubleJumpAllowed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runner Properties")
	float CharacterDoubleJumpCoolDown = 4.0f;

	// Scoring stuff
	UPROPERTY(BlueprintReadOnly, Category = "Side Runner Scoring")
	FText FinalScoreText;
	UPROPERTY(BlueprintReadOnly, Category = "Side Runner Scoring")
	FText HighScoreText;
	UPROPERTY(BlueprintReadOnly, Category = "Side Runner Scoring")
	FText PlayerNamesText;
	UPROPERTY(BlueprintReadOnly, Category = "Side Runner Scoring")
	float CurrentScoreMultiplier;
	UPROPERTY(BlueprintReadOnly, Category = "Side Runner Scoring")
	float CurrentScore;
	UPROPERTY(EditAnywhere, Category = "Side Runner Scoring")
	int32 MaxHighScores;
	UPROPERTY(EditAnywhere, Category = "Side Runner Scoring")
	TArray<int32> HighScoreArray;
	UPROPERTY(EditAnywhere, Category = "Side Runner Scoring")
	TArray<FText> PlayerNameArray;
	FText PlayerNameCurrent;

	// Enum to store whether the game is being played
	EGameState GameState;

	// References to relevant actors
	ARunnerCharacter* PlayerCharacter;
	AKillWall* KillWall;
	ASideRunnerPlayerController* PlayerController;

	UFUNCTION(BlueprintImplementableEvent, Category = "Side Runner Game")
	void DoOnTriggerDeath();

	UFUNCTION(BlueprintCallable, Category = "Side Runner Game")
	void RestartGame();

	void ShowGameOverMenu();

	void SetHighScoreText();

	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FText PlayerName);

	float GetPlayerWallDistance();

	float GetScoreMultiplier();

private:
	static const float DistanceMin;
	static const float DistanceMax;
	static const float AnimSpeedMax;
	static const float AnimSpeedMin;
};



