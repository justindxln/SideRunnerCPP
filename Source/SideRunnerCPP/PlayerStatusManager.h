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
	void ToggleSpeedBuff(const bool bActive);

	// Turn on/off Shield status
	void ToggleShield(const bool bActive);

	// Turn on/off Healing status effect
	void ToggleHealing(const bool bActive);

	void ApplyScoreBoost() const;

	// Apply healing instantly
	void ApplyInstantHealing(const float HealingAmount);

	// Apply damage instantly
	void ApplyInstantDamage(const float DamageAmount);

	// Add damage over time
	void AddDamageOverTime(const float DamagePerSecond);

	// Subtract damage over time
	void SubtractDamageOverTime(const float DamagePerSecond);

	// Called when HP is 0 to restart level
	void TriggerDeath() const;

	// Functions to implement in Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void DoSetHUDReference() const;
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDAddHealth() const;
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDTakeDamage() const;
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDToggleShield(const bool bActive) const;
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDToggleSpeed(const bool bActive) const;
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDToggleHealing(const bool bActive) const;
	UFUNCTION(BlueprintImplementableEvent)
	void DoHUDApplyScoreBoost() const;

public:
	// Called by GameMode after construction to set class references
	void SetClassReferences(ASideRunnerCPPGameMode* const CurrentGameMode, UUserWidget* const CurrentHUDWidget);

	// Call when player collides with a powerup to trigger the effect
	void ReceivePowerUp(const EPowerUpType PowerUpType, const float PowerUpValue);

	// Process Damage value and type and decide how to apply it to player
	void ReceiveDamage(const float DamageValue, const EDamageType DamageType);

	// Call to stop applying damage over time
	void EndDamage(const float DamageValue, const EDamageType DamageType);

	// Get Heal Status duration for the HUD
	UFUNCTION(BlueprintCallable)
	float GetHealPercentage() const;

	// Get Speed duration for the HUD
	UFUNCTION(BlueprintCallable)
	float GetSpeedPercentage() const;

	// Get Shield Percentage for the HUD
	UFUNCTION(BlueprintCallable)
	float GetShieldPercentage() const;

	// Get HP Percentage for the HUD
	UFUNCTION(BlueprintCallable)
	float GetHPPercentage() const;

	// Get HP number text for the HUD
	UFUNCTION(BlueprintCallable)
	FText GetHPText() const;
};
