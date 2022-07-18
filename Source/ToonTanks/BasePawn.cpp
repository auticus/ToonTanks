// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider")); // creating the capsule sub object of the pawn in code vs doing it in blueprint or editor.
	RootComponent = CapsuleCollider; // setting the root component here instead of doing so in the blueprint editor.

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleCollider); //can also pass in RootComponent - its the same thing this is creating this as a child to the Capsule

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh); // creating this as a child to the BaseMesh

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh); // creating this as a child to the Turret
}

/// <summary>
/// Rotates the turret to face a target
/// </summary>
/// <param name="target">The target to face.</param>
void ABasePawn::RotateTurret(FVector target)
{
	// formula to find facing vector is simply V = target - current location
	FVector targetVector = target - TurretMesh->GetComponentLocation(); // note this is a WORLD SPACE ROTATION

	// we only care about the yaw we don't want to mess with pitch or roll
	// can set them at 0 individually OR just use one line
	FRotator lookAtRotation = FRotator(0.f, targetVector.Rotation().Yaw, 0.f);

	//setting the direction rotation like this causes the turret to snap around.  This looks bad.
	//TurretMesh->SetWorldRotation(lookAtRotation); // only changes yaw, and this ia WORLD SPACE ROTATION

	TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), //The component we want to rotate
		lookAtRotation, //the FRotator to work with
		UGameplayStatics::GetWorldDeltaSeconds(this), 
		10.f)); //speed of turret rotation

	// challenge will be to know when to use local vs when to use world as well as to know what function to call to rotate
}

void ABasePawn::Fire()
{
	// invoked by player input or AI code
	// tank class wires this up with player input
	FVector location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator rotation = ProjectileSpawnPoint->GetComponentRotation();

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, location, rotation);
}