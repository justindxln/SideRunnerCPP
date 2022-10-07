// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SideRunnerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNERCPP_API ASideRunnerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SetInputUIOnly();

	void SetInputGameOnly();
};
