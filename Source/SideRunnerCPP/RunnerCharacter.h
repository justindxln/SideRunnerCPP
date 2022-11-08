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
	// Camera control variables
	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float MinCameraTruckRate = 60.f;
	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float CameraTruckRateFactor = 0.75f;
	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float MaxCameraYOffset = 200.f;
	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float MinCameraYOffset = 0.f;
	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float CameraXOffset = -850.f;
	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float InitialCameraZOffset = 300.f;
	float CameraYOffset;
	float CameraZLocation;
	bool IsPlayerFacingRight;

	bool CanMove;

	// Used to keep track of double jump availability
	int32 JumpMaxCountOriginal;
	float DoubleJumpCoolDown;
	float DoubleJumpCoolDownMax;

	// Move left/right
	void MoveRight(float value);

	void RecoverJumpCount();

	// Call every frame to shift camera location based on where the player is facing
	void MoveCamera(float DeltaTime);

public:
	// Get Double Jump Cooldown Progress for the HUD
	UFUNCTION(BlueprintCallable)
	float GetDoubleJumpCoolDownPercentage();

	// Called when HP is 0 to stop movement
	void TriggerDeath();

	// Called by Player Status Manager to toggle Speed Buff
	void ToggleSpeedBuff(bool Active, float BuffAmount);

	void ToggleMovement(bool AllowMovement = true);

	// Called by Game Mode to apply user settings
	void SetupMovementProperties(float RunSpeed, bool DoubleJump, float DoubleJumpCD);
};
