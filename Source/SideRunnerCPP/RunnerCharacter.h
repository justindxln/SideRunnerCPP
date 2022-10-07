// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SideRunnerCPPGameMode.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class SIDERUNNERCPP_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* SideViewCamera;

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Move left/right
	void MoveRight(float value);

public:
	// Return the attached camera
	class UCameraComponent* GetSideViewCameraComponent() const
	{
		return SideViewCamera;
	}

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ToggleMovement(bool AllowMovement = true);

	void SetupMovementProperties(float RunSpeed, bool DoubleJump);

private:
	float zPosition;
	FVector tempPos = FVector();

	bool CanMove;

	void TriggerDeath();
};
