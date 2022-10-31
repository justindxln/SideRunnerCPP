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

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side View Camera"));
	SideViewCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	tempPos = GetActorLocation();
	zPosition = tempPos.Z + 300.f;
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

	// Set camera position
	tempPos = GetActorLocation();
	tempPos.X -= 850.f;
	tempPos.Y += 200.f;
	tempPos.Z = zPosition;
	SideViewCamera->SetWorldLocation(tempPos);

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
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight);
}

void ARunnerCharacter::MoveRight(float value)
{
	if (CanMove) {
		GetCharacterMovement()->bOrientRotationToMovement = true;
		AddMovementInput(FVector(0.f, 1.f, 0.f), value);
		
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Pitch %f"), GetCharacterMovement()->RotationRate.Pitch));
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Roll %f"), GetCharacterMovement()->RotationRate.Roll));
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Yaw %f"), GetCharacterMovement()->RotationRate.Yaw));
	}
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

void ARunnerCharacter::TriggerDeath()
{
	GetMesh()->Deactivate();
	GetMesh()->SetVisibility(false);

	CanMove = false;
	//Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(this))->RestartGame();
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

	DoubleJumpCoolDownMax = DoubleJumpCD;
	DoubleJumpCoolDown = DoubleJumpCoolDownMax;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SETTING RUN SPEED TO %f"), RunSpeed));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("JUMP COUNT = %d"), JumpMaxCount));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DB JUMP CD = %f"), DoubleJumpCoolDown));
}

