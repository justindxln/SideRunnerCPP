// Fill out your copyright notice in the Description page of Project Settings.


#include "Spikes.h"
#include "RunnerCharacter.h"
#include "Components/BoxComponent.h"
#include "PlayerStatusManager.h"

// Sets default values
ASpikes::ASpikes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpikes::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpikes::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASpikes::OnOverlapEnd);
}

// Called every frame
void ASpikes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpikes::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunnerCharacter* PlayerCharacter = Cast<ARunnerCharacter>(OtherActor)) {
		ASideRunnerCPPGameMode* GameMode = Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode) {
			GameMode->PlayerStatusManager->ReceiveDamage(DamageValue, DamageType);
		}
	}
}

void ASpikes::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARunnerCharacter* PlayerCharacter = Cast<ARunnerCharacter>(OtherActor)) {
		ASideRunnerCPPGameMode* GameMode = Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode) {
			GameMode->PlayerStatusManager->EndDamage(DamageValue, DamageType);
		}
	}
}