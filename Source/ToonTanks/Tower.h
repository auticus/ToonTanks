// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;  //overriding the tick function

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ATank* PlayerTank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Statistics", meta = (AllowPrivateAccess = "true"))
	float FireRange = 1000.f; // how far the turret can see and fire at a tank
};
