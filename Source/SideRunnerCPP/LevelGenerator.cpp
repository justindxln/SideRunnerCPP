// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "BaseLevel.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnLevel(true);
	SpawnLevel();
}

void ALevelGenerator::SpawnLevel(bool IsFirstLevel)
{
	// Set default spawn location for first level
	SpawnLocation = FVector(0.f, 1000.f, 0.f);
	SpawnRotation = FRotator(0, 90, 0);

	// If not first level, get spawn location from the last spawned level
	if (!IsFirstLevel) {
		ABaseLevel* LastLevel = LevelList.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}

	ABaseLevel* NewLevel = nullptr;
	RandomLevelIndex = FMath::RandRange(0, 4);

	// Create a level from a random index in the library
	if (RandomLevelIndex < LevelDatabase.Num() && LevelDatabase[RandomLevelIndex] != nullptr) {
		NewLevel = GetWorld()->SpawnActor<ABaseLevel>(LevelDatabase[RandomLevelIndex], 
			SpawnLocation, SpawnRotation, SpawnParameters);
	}

	// Set the collision trigger events
	if (NewLevel) {
		if (NewLevel->GetTrigger()) {
			NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ALevelGenerator::OnOverlapBegin);
			NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(NewLevel, &ABaseLevel::OnOverlapBegin);
		}
	}

	// Add level to list and destroy oldest one if the list is too big
	LevelList.Add(NewLevel);
	if (LevelList.Num() > MaxLevelAmount) {
		GetWorld()->DestroyActor(LevelList[0]);
		LevelList.RemoveAt(0);
	}
}

void ALevelGenerator::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnLevel();
}

