// Fill out your copyright notice in the Description page of Project Settings.


#include "KillWall.h"

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

	SetActorLocation(GetActorLocation() + FVector(0.f, MoveSpeed *DeltaTime, 0.f), true);

}

void AKillWall::SetMoveSpeed(float NewMoveSpeed)
{
	MoveSpeed = NewMoveSpeed;
}
