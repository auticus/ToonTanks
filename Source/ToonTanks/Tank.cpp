// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Constructs an ATank object
ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent); // childs this to the root (which is the capsule collider)

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(SpringArmComponent);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// search for the mouse cursor every frame
	if (PlayerControllerRef == nullptr) return;

	FHitResult hitResult;
	PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,
		false, // if true will try to calculate the complex wireframe, otherwise if false uses the simple version which is lighter
		hitResult);

	/* Debug Sphere code here for future reference for learning.
	DrawDebugSphere(GetWorld(),
		hitResult.ImpactPoint,
		25.f, //radius
		12, //segments
		FColor::Red,
		false, //persistant lines
		-1.f); //getting a sphere every frame
	*/
	
	RotateTurret(hitResult.ImpactPoint);
}

APlayerController* ATank::GetPlayerController() const
{
	return PlayerControllerRef;
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	//GetController() will return a type of AController*
	//You cannot just assign AController to APlayerController (even though player controller is a child of Acontroller)
	//However we know that the controller coming back is a Player Controller for this so we perform a cast
	PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);  // maps the MoveForward axis to this tank, and maps it to the Move function here.
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	// do I need mouse input?  The player controller already has this.

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire); //IE_Released would bind this if it was released instead of pressed
}

/// <summary>
/// Method that will move the tank forward and backward and is mapped to the MoveForward bound axis of the input handler.
/// </summary>
/// <param name="Value">A value indicating forward (1.0) or backward (-1.0)</param>
void ATank::Move(float Value)
{
	FVector Direction = FVector::ZeroVector;
	Direction.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);  // sets this to be (1,0,0) or (-1,0,0) and multiplies by speed and delta time
	AddActorLocalOffset(Direction, true); // why use local?  the x of the vector represents the local object's forward and backward direction.  World likely does not.
									// if using world space, you'd have to figure out what the forward vector is in world space of the actor first.
									// the true value indicates it should sweep for collisions and if you have set to block that type of object, it will stop
									// MAKE SURE YOUR COLLIDER IS NOT IN THE FLOOR OR YOU WILL NOT MOVE!!!
}

/// <summary>
/// Method that will turn the tank right or left.
/// </summary>
/// <param name="Value">A value indicating right (1.0) or left (-1.0)</param>
void ATank::Turn(float Value)
{
	FRotator Direction = FRotator::ZeroRotator;
	Direction.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(Direction, true);
}