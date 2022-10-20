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

	if (CanMove) {
	SetActorLocation(GetActorLocation() + FVector(0.f, MoveSpeed *DeltaTime, 0.f), true);
	}
}

void AKillWall::SetMoveSpeed(float NewMoveSpeed)
{
	MoveSpeed = NewMoveSpeed;
}

void AKillWall::SetCanMove(bool bCanMove /*= true*/)
{
	CanMove = bCanMove;
}
