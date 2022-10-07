// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseLevel.generated.h"

class UBoxComponent;

UCLASS()
class SIDERUNNERCPP_API ABaseLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Triggers")
	UBoxComponent* Trigger;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Triggers")
	UBoxComponent* SpawnLocation;
	
public:
	UBoxComponent* GetTrigger();
	UBoxComponent* GetSpawnLocation();


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Disable box after it has been triggered so it is only used once
	void DisableTrigger();
};
