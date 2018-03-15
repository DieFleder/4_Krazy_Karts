// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"
#include "Components/InputComponent.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
	
}

void AGoKart::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGoKart, ReplicatedTransform);
	DOREPLIFETIME(AGoKart, Velocity);
	DOREPLIFETIME(AGoKart, Throttle);
	DOREPLIFETIME(AGoKart, SteeringThrow);
}

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "Other";
	}
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * Throttle;
	Force += GetAirResistance() + GetRollingResistance();

	FVector Acceleration = Force / Mass;

	FVector InitialVelocity = Velocity;
	Velocity = Velocity + Acceleration * DeltaTime; // incriment velocity

	UpdateTransform(DeltaTime);

	if (HasAuthority())
	{
		ReplicatedTransform = GetActorTransform();
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}

void AGoKart::OnRep_ReplicatedTransform()
{
		SetActorTransform(ReplicatedTransform);
}

void AGoKart::UpdateTransform(float DeltaTime)
{
	/* Calculate the change in rotation */
	float DeltaLocation = FVector::DotProduct(Velocity, GetActorForwardVector()) * DeltaTime;
	float RotationAngle = DeltaLocation * SteeringThrow / (TurnRadius * 100);
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	/* Calculate the change in location */
	FVector TranslationDelta = Velocity * 100 * DeltaTime; // multiply by 100 to convert from cm to meters

	/*Apply the change to the transform*/
	FTransform DeltaTransform = FTransform(RotationDelta, TranslationDelta);

	FHitResult HitResult;
	AddActorWorldTransform(DeltaTransform, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = Velocity * 0.f; // stop the kart if it hits something
	}
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::MoveForward(float AxisInValue)
{
	Throttle = AxisInValue * MaxThrottle;
	Server_MoveForward(AxisInValue);
}

void AGoKart::MoveRight(float AxisInValue)
{
	SteeringThrow = AxisInValue;
	Server_MoveRight(AxisInValue);
}

void AGoKart::Server_MoveForward_Implementation(float AxisInValue)
{
	Throttle = AxisInValue * MaxThrottle;
}

bool AGoKart::Server_MoveForward_Validate(float AxisInValue)
{
	return FMath::Abs(AxisInValue) <= 1;
}

void AGoKart::Server_MoveRight_Implementation(float AxisInValue)
{
	SteeringThrow = AxisInValue;
}

bool AGoKart::Server_MoveRight_Validate(float AxisInValue)
{
	return FMath::Abs(AxisInValue) <= 1;
}

FVector AGoKart::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * AirDragCoefficient;
}

FVector AGoKart::GetRollingResistance()
{
	float AccelerationDueToGravity = GetWorld()->GetGravityZ() / 100;

	/*
	 *AccelerationDueToGravity is a negative number with UE4 defaults.
	 *Therefore, no need to multiply the return value by -1
	 */
	return Velocity.GetSafeNormal() * Mass * AccelerationDueToGravity * RollingFrictionCoefficient; //
}