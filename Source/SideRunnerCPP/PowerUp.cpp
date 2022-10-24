// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "RunnerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlapBegin);
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunnerCharacter* PlayerCharacter = Cast<ARunnerCharacter>(OtherActor)) {
		ASideRunnerCPPGameMode* GameMode = Cast<ASideRunnerCPPGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode) {
			GameMode->PlayerStatusManager->ReceivePowerUp(PowerUpType, PowerUpValue);
		}

		Destroy();
	}
}

