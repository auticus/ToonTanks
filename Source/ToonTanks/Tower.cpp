// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	// find the player tank and store its pointer
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerTank == nullptr) return;

	//rotate the turret if tank is in range and face the tank
	FVector playerLocation = PlayerTank->GetActorLocation();
	float Distance = FVector::Dist(GetActorLocation(), PlayerTank->GetActorLocation());
	if (Distance > FireRange) return;

	RotateTurret(playerLocation);
}