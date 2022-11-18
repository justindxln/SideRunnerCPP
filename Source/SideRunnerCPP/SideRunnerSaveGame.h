// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SideRunnerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNERCPP_API USideRunnerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "PLayerSettings")
	float PlayerSpeed;

	UPROPERTY(VisibleAnywhere, Category = "PLayerSettings")
	bool bPlayerDoubleJump;

	UPROPERTY(VisibleAnywhere, Category = "PLayerSettings")
	float PlayerDoubleJumpCoolDown;

	UPROPERTY(VisibleAnywhere, Category = "PLayerSettings")
	float WallSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Score")
	TArray<int32> HighScores;

	UPROPERTY(VisibleAnywhere, Category = "Score")
	TArray<FText> PlayerNames;
};
