// Fill out your copyright notice in the Description page of Project Settings.


#include "ToontanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToontanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
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
		GameOver(false);
	}
	else if (ATower* destroyedTower = Cast<ATower>(deadActor))
	{
		destroyedTower->HandleDestruction();
		--TargetTowers;

		if (TargetTowers == 0)
		{
			GameOver(true);
		}
	}
}

void AToontanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame(); //implemented via blueprint

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;

		// here we have a delegate that takes a function that also has input parameters
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController, // the object that houses the function
			&AToonTanksPlayerController::SetPlayerEnabledState, // the reference to the function with the input 
			true // the input
		);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false); //false means do not loop
	}
}

int32 AToontanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> enemyTowers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), enemyTowers);
	return enemyTowers.Num();
}