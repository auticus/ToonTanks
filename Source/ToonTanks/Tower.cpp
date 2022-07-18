// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	// find the player tank and store its pointer
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireControlTimer, this, &ATower::CheckFireCondition, FireRate, true);  //true is for loop otherwise it only does it once.
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsInRange()) return;

	FVector playerLocation = PlayerTank->GetActorLocation();
	RotateTurret(playerLocation);
}

void ATower::CheckFireCondition()
{
	if (IsInRange()) Fire();
}

bool ATower::IsInRange()
{
	if (PlayerTank == nullptr) return false;
	float distanceToTank = FVector::Dist(GetActorLocation(), PlayerTank->GetActorLocation());
	return distanceToTank <= FireRange;
}