// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumName.h"
#include "PlayerStatusManager.generated.h"

class ASideRunnerCPPGameMode;

// This class manages all player HP and status effects related functionalities
UCLASS()
class SIDERUNNERCPP_API APlayerStatusManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStatusManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	float HPMax = 100.f;
	float HPCurrent;

	// The damage over time player will take per second while it's active
	float DamagePerSecondCurrent;

	// Apply damage instantly
	void ApplyInstantDamage(float DamageAmount);

	// Add damage over time
	void AddDamageOverTime(float DamagePerSecond);

	// Subtract damage over time
	void SubtractDamageOverTime(float DamagePerSecond);

	// Called when HP is 0 to restart level
	void TriggerDeath();

public:
	ASideRunnerCPPGameMode* GameMode;

	// Process Damage value and type and decide how to apply it to player
	void ReceiveDamage(float DamageValue, EDamageType DamageType);

	// Call to stop applying damage over time
	void EndDamage(float DamageValue, EDamageType DamageType);

	UFUNCTION(BlueprintCallable)
	float GetHPPercentage();

	UFUNCTION(BlueprintCallable)
	FText GetHPText();
};
