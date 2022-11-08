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

	TArray<ABaseLevel*> LevelList;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseLevel>> LevelDatabase;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseLevel>> TestModeDatabase;

	UPROPERTY(EditAnywhere)
	FVector DefaultSpawnLocation = FVector(0.f, 1000.f, 0.f);

	UPROPERTY(EditAnywhere)
	FRotator DefaultSpawnRotation = FRotator(0, 90, 0);

	int RandomLevelIndex;
};
