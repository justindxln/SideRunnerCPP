// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "BaseLevel.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "SideRunnerCPPGameMode.h"

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
	
	SpawnLevel();
	SpawnLevel();
}

void ALevelGenerator::SpawnLevel()
{
	// Set default spawn location for first level
	FVector SpawnLocation = DefaultSpawnLocation;
	FRotator SpawnRotation = DefaultSpawnRotation;
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();

	// If not first level, get spawn location from the last spawned level
	if (LevelList.Num() > 0) {
		ABaseLevel* LastLevel = LevelList.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}

	ABaseLevel* NewLevel = nullptr;

	// Select the default Level Database, unless the game is in test mode, then select the Test Mode Level Database
	TArray<TSubclassOf<ABaseLevel>> Database = LevelDatabase;
	if (ASideRunnerCPPGameMode* GameMode = Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(this))) {
		if (GameMode->IsTestMode) Database = TestModeDatabase;
	}

	RandomLevelIndex = FMath::RandRange(0, Database.Num() - 1);

	// Create a level from a random index in the library
	if (Database[RandomLevelIndex] != nullptr) {
		NewLevel = GetWorld()->SpawnActor<ABaseLevel>(Database[RandomLevelIndex], 
			SpawnLocation, SpawnRotation, SpawnParameters);
	}

	// Set the collision trigger events
	if (NewLevel) {
		if (NewLevel->GetTrigger()) {
			NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(NewLevel, &ABaseLevel::OnOverlapBegin);
			NewLevel->OnOverlapPlayer.BindDynamic(this, &ALevelGenerator::SpawnLevel);
			NewLevel->OnOverlapWall.BindDynamic(this, &ALevelGenerator::DestroyOldestLevel);
		}
	}

	LevelList.Add(NewLevel);
}

void ALevelGenerator::DestroyOldestLevel()
{
	GetWorld()->DestroyActor(LevelList[0]);
	LevelList.RemoveAt(0);
}