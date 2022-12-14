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
	const FRotator SpawnRotation = DefaultSpawnRotation;
	const FActorSpawnParameters SpawnParameters = FActorSpawnParameters();

	// If not first level, get spawn location from the last spawned level
	if (LevelList.Num() > 0)
	{
		const ABaseLevel* const LastLevel = LevelList.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}

	// Select the default Level Database, unless the game is in test mode, then select the Test Mode Level Database
	TArray<TSubclassOf<ABaseLevel>> Database = LevelDatabase;
	if (ASideRunnerCPPGameMode* const GameMode = Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(this))) 
	{
		if (GameMode->bIsTestMode) Database = TestModeDatabase;
	}

	const int RandomLevelIndex = FMath::RandRange(0, Database.Num() - 1);

	if (Database[RandomLevelIndex] == nullptr) return;
	
	// Create a level from a random index in the library
	ABaseLevel* const NewLevel = GetWorld()->SpawnActor<ABaseLevel>(Database[RandomLevelIndex],
		SpawnLocation, SpawnRotation, SpawnParameters);
	
	if (!NewLevel || !NewLevel->GetTrigger()) return;
	
	// Set the collision trigger events
	NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(NewLevel, &ABaseLevel::OnOverlapBegin);
	NewLevel->OnOverlapPlayer.BindDynamic(this, &ALevelGenerator::SpawnLevel);
	NewLevel->OnOverlapWall.BindDynamic(this, &ALevelGenerator::DestroyOldestLevel);

	LevelList.Add(NewLevel);
}

void ALevelGenerator::DestroyOldestLevel()
{
	if (LevelList.Num() <= 0 || LevelList[0] == nullptr) return;

	GetWorld()->DestroyActor(LevelList[0]);
	LevelList.RemoveAt(0);
}