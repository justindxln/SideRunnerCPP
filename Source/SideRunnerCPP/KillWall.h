// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spikes.h"
#include "KillWall.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNERCPP_API AKillWall : public ASpikes
{
	GENERATED_BODY()
	
public:
	AKillWall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	// Allows Game Mode to adjust Move Speed
	void SetMoveSpeed(float NewMoveSpeed);

	// Called when level begins so the wall starts moving
	void SetCanMove(bool bCanMove = true);

private:
	float MoveSpeed = 350.0f;
	bool CanMove = false;
};
