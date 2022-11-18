// Fill out your copyright notice in the Description page of Project Settings.


#include "KillWall.h"
#include "RunnerCharacter.h"

AKillWall::AKillWall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKillWall::BeginPlay()
{
	Super::BeginPlay();

	// VARIABLE FOR LATER
	this->GetRootComponent()->ComponentVelocity = FVector(0.f, 25.f, 0.f);
}

void AKillWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanMove)
	{
		SetActorLocation(GetActorLocation() + FVector(0.f, MoveSpeed *DeltaTime, 0.f), true);
	}
}

void AKillWall::SetMoveSpeed(const float NewMoveSpeed)
{
	MoveSpeed = NewMoveSpeed;
}

void AKillWall::SetCanMove(const bool bInCanMove /*= true*/)
{
	bCanMove = bInCanMove;
}

void AKillWall::BoostMoveSpeed(const float ExtraMoveSpeed)
{
	MoveSpeed += ExtraMoveSpeed;
}
