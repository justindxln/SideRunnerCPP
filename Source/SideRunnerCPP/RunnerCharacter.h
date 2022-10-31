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

	// Process Jump command and decide whether to allow jump
	virtual void OnJumped_Implementation() override;

protected:
	// Position info to place the camera
	float zPosition;
	FVector tempPos = FVector();

	bool CanMove;

	// The damage over time player will take per second while it's active
	float DamageContinuous;

	// Used to keep track of double jump availability
	int32 JumpMaxCountOriginal;
	float DoubleJumpCoolDown;
	float DoubleJumpCoolDownMax;

	// Move left/right
	void MoveRight(float value);

	void RecoverJumpCount();

public:
	// Return the attached camera
	class UCameraComponent* GetSideViewCameraComponent() const
	{
		return SideViewCamera;
	}

	UFUNCTION(BlueprintCallable)
	float GetDoubleJumpCoolDownPercentage();

	// Called when HP is 0 to stop movement
	void TriggerDeath();

	// Called by Player Status Manager to toggle Speed Buff
	void ToggleSpeedBuff(bool Active, float BuffAmount);

	void ToggleMovement(bool AllowMovement = true);

	void SetupMovementProperties(float RunSpeed, bool DoubleJump, float DoubleJumpCD);
};
