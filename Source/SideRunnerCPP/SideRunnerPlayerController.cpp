// Fill out your copyright notice in the Description page of Project Settings.


#include "SideRunnerPlayerController.h"

void ASideRunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputUIOnly();
}

void ASideRunnerPlayerController::SetInputUIOnly()
{
	//SetInputMode(FInputModeUIOnly());
}

void ASideRunnerPlayerController::SetInputGameOnly()
{
	//SetInputMode(FInputModeGameOnly());
}
