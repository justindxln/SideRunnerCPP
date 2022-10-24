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

// Called every frame
void APlayerStatusManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DamagePerSecondCurrent > 0) {
		ApplyInstantDamage(DamagePerSecondCurrent * DeltaTime);
	}

	if (ShieldCurrent > 0) {
		ShieldCurrent = FMath::Clamp(ShieldCurrent - ShieldDecayRate * DeltaTime, 0.f, ShieldMax);
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
		ApplyShield(PowerUpValue);
		break;
	}


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

void APlayerStatusManager::ApplyShield(float ShieldAmount)
{
	ShieldCurrent = FMath::Clamp(ShieldCurrent + ShieldAmount, 0.f, ShieldMax);
}

void APlayerStatusManager::ApplyInstantHealing(float HealingAmount)
{
	HPCurrent = FMath::Clamp(HPCurrent += HealingAmount, 0.f, HPMax);
}

void APlayerStatusManager::ApplyInstantDamage(float DamageAmount)
{
	if (HPCurrent <= 0.f) return;

	// Shield can block damage before it breaks. Extra damage does not spill over to HP
	if (ShieldCurrent > 0) {
		ShieldCurrent = FMath::Clamp(ShieldCurrent - DamageAmount, 0.f, ShieldMax);
		return;
	}

	// Apply instant point damage
	HPCurrent = FMath::Clamp(HPCurrent - DamageAmount, 0.f, HPMax);

	// Player dies if health below zero
	if (HPCurrent <= 0) {
		Cast<ARunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->TriggerDeath();

		TriggerDeath();
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

void APlayerStatusManager::TriggerDeath()
{
	GameMode->TriggerDeath();
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

FLinearColor APlayerStatusManager::GetHPColor()
{
	FLinearColor MaxColor = FLinearColor::Green;
	FLinearColor MinColor = FLinearColor::Red;

	return FLinearColor::LerpUsingHSV(MinColor, MaxColor, GetHPPercentage());
}
