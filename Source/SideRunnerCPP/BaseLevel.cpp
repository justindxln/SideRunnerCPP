// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLevel.h"
#include "Components/BoxComponent.h"
#include "RunnerCharacter.h"
#include "KillWall.h"

// Sets default values
ABaseLevel::ABaseLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseLevel::BeginPlay()
{
	Super::BeginPlay();
	
	if (Trigger != nullptr) Trigger->bHiddenInGame = true;
}

// Called every frame
void ABaseLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBoxComponent* ABaseLevel::GetTrigger() const
{
	return Trigger;
}

UBoxComponent* ABaseLevel::GetSpawnLocation() const
{
	return SpawnLocation;
}

void ABaseLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunnerCharacter* Player = Cast<ARunnerCharacter>(OtherActor))
	{
		if (bSpawnTriggerUsed) return;

		OnOverlapPlayer.ExecuteIfBound();
		bSpawnTriggerUsed = true;
	}
	else if (AKillWall* KillWall = Cast<AKillWall>(OtherActor))
	{
		OnOverlapWall.ExecuteIfBound();
	}
}

