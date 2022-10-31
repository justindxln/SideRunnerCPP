// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

class ABaseLevel;

UCLASS()
class SIDERUNNERCPP_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SpawnLevel();

	// Destroy oldest level, called when the wall passes this level
	UFUNCTION()
	void DestroyOldestLevel();

protected:

	APawn* Player;
	TArray<ABaseLevel*> LevelList;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseLevel>> LevelDatabase;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseLevel>> TestModeDatabase;

	// Maximum number of levels in the scene before oldest ones get destroyed
	UPROPERTY(EditAnywhere)
	int MaxLevelAmount = 10;

public:

	int RandomLevelIndex;

	FVector SpawnLocation = FVector();
	FRotator SpawnRotation = FRotator();
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
};
