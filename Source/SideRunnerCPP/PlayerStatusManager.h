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
	float ShieldMax = 100.f;
	UPROPERTY(EditAnywhere)
	float ShieldDecayRate = 5.f;
	UPROPERTY(EditAnywhere)
	float HPMax = 100.f;

	float ShieldCurrent;
	float HPCurrent;
	float ArmorCurrent;

	// The damage over time player will take per second while it's active
	float DamagePerSecondCurrent;


	// Add Shield Value
	void ApplyShield(float ShieldAmount);

	// Apply healing instantly
	void ApplyInstantHealing(float HealingAmount);

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

	// Call when player collides with a powerup to trigger the effect
	void ReceivePowerUp(EPowerUpType PowerUpType, float PowerUpValue);

	// Process Damage value and type and decide how to apply it to player
	void ReceiveDamage(float DamageValue, EDamageType DamageType);

	// Call to stop applying damage over time
	void EndDamage(float DamageValue, EDamageType DamageType);

	// Get Shield Percentage for the HUD
	UFUNCTION(BlueprintCallable)
	float GetShieldPercentage();

	// Get HP Percentage for the HUD
	UFUNCTION(BlueprintCallable)
	float GetHPPercentage();

	// Get HP number text for the HUD
	UFUNCTION(BlueprintCallable)
	FText GetHPText();

	// Get color for HP bar for the HUD
	UFUNCTION(BlueprintCallable)
	FLinearColor GetHPColor();
};
