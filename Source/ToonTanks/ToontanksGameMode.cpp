// Fill out your copyright notice in the Description page of Project Settings.


#include "ToontanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToontanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void AToontanksGameMode::ActorDied(AActor* deadActor)
{
	if (deadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
	}
	else if (ATower* destroyedTower = Cast<ATower>(deadActor))
	{
		destroyedTower->HandleDestruction();
	}
}