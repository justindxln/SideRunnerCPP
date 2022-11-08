// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Spikes.h"
#include "KillWall.h"
#include "Engine.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side View Camera"));
	SideViewCamera->bUsePawnControlRotation = false;

	/*
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	*/

	CameraZLocation = GetActorLocation().Z + InitialCameraZOffset;
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ToggleMovement(false);

}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera(DeltaTime);

	// Recover double jump if applicable and on cooldown
	if (JumpMaxCountOriginal > 1 && DoubleJumpCoolDown < DoubleJumpCoolDownMax) {
		DoubleJumpCoolDown += DeltaTime;

		if (DoubleJumpCoolDown >= DoubleJumpCoolDownMax) {
			RecoverJumpCount();
		}
	}
}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunnerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight);
}

void ARunnerCharacter::MoveRight(float value)
{
	if (CanMove) {
		GetCharacterMovement()->bOrientRotationToMovement = true;
		AddMovementInput(FVector(0.f, 1.f, 0.f), value);
	}

	// Move the camera depending on where the player is going
	// If no movement is detected, maintain camera direction
	if (value > 0) IsPlayerFacingRight = true;
	else if (value < 0) IsPlayerFacingRight = false;
}

void ARunnerCharacter::OnJumped_Implementation()
{
	// If it's a double (or more) jump, decrease the max jump count and put it on cooldown
	if (JumpCurrentCount > 1) {
		JumpMaxCount--;
		DoubleJumpCoolDown = 0.f;
	}
}

// After set duration, recover the max jump count to allow double jump again
void ARunnerCharacter::RecoverJumpCount()
{
	JumpMaxCount++;
}

void ARunnerCharacter::MoveCamera(float DeltaTime)
{
	// Set shift speed based on how far the camera is from target destination
	float YDistanceFromTarget = IsPlayerFacingRight ? MaxCameraYOffset - CameraYOffset : CameraYOffset - MinCameraYOffset;
	float CameraTruckRate = YDistanceFromTarget * CameraTruckRateFactor;

	// Shift camera to the left/right depending on player direction, then clamp within set limits
	CameraYOffset = CameraYOffset + (DeltaTime * (IsPlayerFacingRight ? CameraTruckRate : -CameraTruckRate));
	CameraYOffset = FMath::Clamp(CameraYOffset, 0.f, MaxCameraYOffset);

	// Set camera location by applying offset vector to player location
	// Z location is fixed so camera doesn't move when the player jumps
	FVector CameraLocation = GetActorLocation() + FVector(CameraXOffset, CameraYOffset, 0.f);
	CameraLocation.Z = CameraZLocation;
	SideViewCamera->SetWorldLocation(CameraLocation);
}

// Disable movement and collision and hide mesh when HP is 0
void ARunnerCharacter::TriggerDeath()
{
	GetMesh()->Deactivate();
	GetMesh()->SetVisibility(false);

	CanMove = false;
}

void ARunnerCharacter::ToggleSpeedBuff(bool Active, float BuffAmount)
{
	GetCharacterMovement()->MaxWalkSpeed += Active ? BuffAmount : -BuffAmount;
	GetCharacterMovement()->MaxFlySpeed += Active ? BuffAmount : -BuffAmount;
}

float ARunnerCharacter::GetDoubleJumpCoolDownPercentage()
{
	return FMath::Clamp(DoubleJumpCoolDown / DoubleJumpCoolDownMax, 0.0f, 1.0f);
}

void ARunnerCharacter::ToggleMovement(bool AllowMovement /*= true*/)
{
	CanMove = AllowMovement;
}

void ARunnerCharacter::SetupMovementProperties(float RunSpeed, bool DoubleJump, float DoubleJumpCD)
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxFlySpeed = RunSpeed;
	JumpMaxCountOriginal = DoubleJump ? 2 : 1;
	JumpMaxCount = JumpMaxCountOriginal;

	this->DoubleJumpCoolDownMax = DoubleJumpCD;
	this->DoubleJumpCoolDown = DoubleJumpCoolDownMax;
}

