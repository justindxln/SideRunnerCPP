// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusManager.h"
#include "SideRunnerCPPGameMode.h"
#include "RunnerCharacter.h"

// Sets default values
APlayerStatusManager::APlayerStatusManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameMode = Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called when the game starts or when spawned
void APlayerStatusManager::BeginPlay()
{
	Super::BeginPlay();
	
	HPCurrent = HPMax;
}

void APlayerStatusManager::SetClassReferences(ASideRunnerCPPGameMode* CurrentGameMode, UUserWidget* CurrentHUDWidget)
{
	GameMode = CurrentGameMode;
	HUDWidget = CurrentHUDWidget;

	DoSetHUDReference();
}

// Called every frame
void APlayerStatusManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DamagePerSecondCurrent > 0) {
		ApplyInstantDamage(DamagePerSecondCurrent * DeltaTime);
	}

	if (SpeedDuration > 0) {
		SpeedDuration = FMath::Clamp(SpeedDuration - DeltaTime, 0.f, SpeedDurationMax);
		if (SpeedDuration <= 0.f) {
			ToggleSpeedBuff(false);
		}
	}

	if (HealStatusDuration > 0) {
		HealStatusDuration = FMath::Clamp(HealStatusDuration - DeltaTime, 0.f, HealStatusDurationMax);
		ApplyInstantHealing(HealStatusPerSecond * DeltaTime);

		if (HealStatusDuration <= 0.f) {
			ToggleHealing(false);
		}
	}
}

void APlayerStatusManager::ReceivePowerUp(EPowerUpType PowerUpType, float PowerUpValue)
{
	switch (PowerUpType)
	{
	case EPowerUpType::Health:
		ApplyInstantHealing(PowerUpValue);
		break;
	case EPowerUpType::Shield:
		ToggleShield(true);
		break;
	case EPowerUpType::Speed:
		ToggleSpeedBuff(true);
		break;
	case EPowerUpType::Healing:
		ToggleHealing(true);
		break;
	case EPowerUpType::ScoreBoost:
		ApplyScoreBoost();
		break;
	}
}

void APlayerStatusManager::ToggleSpeedBuff(bool Active)
{
	// Refresh the buff, but to not apply the speed increase again if the buff is already active
	bool BuffRefresh = Active && SpeedDuration > 0;

	if (Active) {
		SpeedDuration = SpeedDurationMax;
	}
	if (BuffRefresh) return;

	Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->ToggleSpeedBuff(Active, SpeedBuffAmount);

	DoHUDToggleSpeed(Active);
}

void APlayerStatusManager::ToggleShield(bool Active)
{
	if (Active) {
		ShieldCurrent = FMath::Clamp(ShieldCurrent + 100.0f, 0.f, ShieldMax);
	}
	else {
		ShieldCurrent = 0;
	}

	DoHUDToggleShield(Active);
}

void APlayerStatusManager::ToggleHealing(bool Active)
{
	if (Active) {
		HealStatusDuration = HealStatusDurationMax;
	}

	DoHUDToggleHealing(Active);
}

void APlayerStatusManager::ApplyScoreBoost()
{
	GameMode->TriggerScoreBoost();

	DoHUDApplyScoreBoost();
}

void APlayerStatusManager::ApplyInstantHealing(float HealingAmount)
{
	HPCurrent = FMath::Clamp(HPCurrent += HealingAmount, 0.f, HPMax);

	DoHUDAddHealth();
}

void APlayerStatusManager::ReceiveDamage(float DamageValue, EDamageType DamageType)
{
	if (DamageType == EDamageType::Wall) {
		AddDamageOverTime(DamageValue);
	}
	else if (DamageType == EDamageType::Spike) {
		ApplyInstantDamage(DamageValue);
	}
}

void APlayerStatusManager::EndDamage(float DamageValue, EDamageType DamageType)
{
	if (DamageType == EDamageType::Wall) {
		SubtractDamageOverTime(DamageValue);
	}
}

void APlayerStatusManager::AddDamageOverTime(float DamagePerSecond)
{
	DamagePerSecondCurrent += DamagePerSecond;
}

void APlayerStatusManager::SubtractDamageOverTime(float DamagePerSecond)
{
	DamagePerSecondCurrent -= DamagePerSecond;
}

void APlayerStatusManager::ApplyInstantDamage(float DamageAmount)
{
	if (HPCurrent <= 0.f) return;

	// Shield can 1 hit before it breaks
	if (ShieldCurrent > 0) {
		ToggleShield(false);
		return;
	}

	// Cancel Heal Status when the player takes damage
	if (HealStatusDuration > 0.f) {
		HealStatusDuration = 0.f;
		ToggleHealing(false);
	}

	// Apply instant point damage
	HPCurrent = FMath::Clamp(HPCurrent - DamageAmount, 0.f, HPMax);
	DoHUDTakeDamage();

	// Player dies if health below zero
	if (HPCurrent <= 0) {
		// Call function on Runner Character to stop movement and hide player mesh
		Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->TriggerDeath();

		TriggerDeath();
	}
}

void APlayerStatusManager::TriggerDeath()
{
	GameMode->TriggerDeath();
}

// Get Heal Status duration for the HUD
float APlayerStatusManager::GetHealPercentage()
{
	return HealStatusDuration / HealStatusDurationMax;
}

// Get Speed duration for the HUD
float APlayerStatusManager::GetSpeedPercentage()
{
	return SpeedDuration / SpeedDurationMax;
}

// Get Shield Percentage for the HUD
float APlayerStatusManager::GetShieldPercentage()
{
	return ShieldCurrent / ShieldMax;
}

// Get HP Percentage for the HUD
float APlayerStatusManager::GetHPPercentage()
{
	return HPCurrent / HPMax;
}

// Get HP Text for the HUD
FText APlayerStatusManager::GetHPText()
{
	return FText::FromString(FString::SanitizeFloat(FMath::RoundToFloat(HPCurrent)));
}