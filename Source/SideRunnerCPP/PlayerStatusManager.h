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
	UPROPERTY(EditAnywhere, Category = "Status Values")
	float HealStatusPerSecond = 4.0f;
	UPROPERTY(EditAnywhere, Category = "Status Durations")
	float HealStatusDurationMax = 10;
	UPROPERTY(EditAnywhere, Category = "Status Values")
	float SpeedBuffAmount = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Status Durations")
	float SpeedDurationMax = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Unused")
	float ShieldMax = 100.f;
	UPROPERTY(EditAnywhere, Category = "Unused")
	float ShieldDecayRate = 5.f;
	UPROPERTY(EditAnywhere, Category = "Base Character Stats")
	float HPMax = 100.f;

	ASideRunnerCPPGameMode* GameMode;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* HUDWidget;

	float HealStatusDuration;
	float SpeedDuration;
	float ShieldCurrent;
	float HPCurrent;
	float ArmorCurrent;

	// The damage over time player will take per second while it's active
	float DamagePerSecondCurrent;

	// Turn on/off Movement Speed buff
	void ToggleSpeedBuff(bool Active);

	// Turn on/off Shield status
	void ToggleShield(bool Active);

	// Turn on/off Healing status effect
	void ToggleHealing(bool Active);

	void ApplyScoreBoost();

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

	// Functions to implement in Blueprint class
	UFUNCTION(BlueprintImplementableEvent)
	void DoSetHUDReference();
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDAddHealth();
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDTakeDamage();
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDToggleShield(bool Active);
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDToggleSpeed(bool Active);
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDToggleHealing(bool Active);
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDApplyScoreBoost();

public:
	// Called by GameMode after construction to set class references
	void SetClassReferences(ASideRunnerCPPGameMode* CurrentGameMode, UUserWidget* CurrentHUDWidget);

	// Call when player collides with a powerup to trigger the effect
	void ReceivePowerUp(EPowerUpType PowerUpType, float PowerUpValue);

	// Process Damage value and type and decide how to apply it to player
	void ReceiveDamage(float DamageValue, EDamageType DamageType);

	// Call to stop applying damage over time
	void EndDamage(float DamageValue, EDamageType DamageType);

	// Get Heal Status duration for the HUD
	UFUNCTION(BlueprintCallable)
	float GetHealPercentage();

	// Get Speed duration for the HUD
	UFUNCTION(BlueprintCallable)
	float GetSpeedPercentage();

	// Get Shield Percentage for the HUD
	UFUNCTION(BlueprintCallable)
	float GetShieldPercentage();

	// Get HP Percentage for the HUD
	UFUNCTION(BlueprintCallable)
	float GetHPPercentage();

	// Get HP number text for the HUD
	UFUNCTION(BlueprintCallable)
	FText GetHPText();
};
