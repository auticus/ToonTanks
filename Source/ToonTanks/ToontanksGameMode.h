// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToontanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToontanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void ActorDied(AActor* deadActor);

protected:
	virtual void BeginPlay() override;

private:
	class ATank* Tank;
	class AToonTanksPlayerController* ToonTanksPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();
};
