// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKartMovementComponent.h"



// Sets default values for this component's properties
UGoKartMovementComponent::UGoKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGoKartMovementComponent::SimulateMove(const FGoKartMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * Move.Throttle;
	Force += GetAirResistance() + GetRollingResistance();

	FVector Acceleration = Force / Mass;

	FVector InitialVelocity = Velocity;
	Velocity = Velocity + Acceleration * Move.DeltaTime; // incriment velocity

	UpdateTransform(Move);
}

FGoKartMove UGoKartMovementComponent::CreateMove(float DeltaTime)
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = FPlatformTime::Seconds();
	return Move;
}

void UGoKartMovementComponent::UpdateTransform(FGoKartMove Move)
{
	/* Calculate the change in rotation */
	float DeltaLocation = FVector::DotProduct(Velocity, GetOwner()->GetActorForwardVector()) * Move.DeltaTime;
	float RotationAngle = DeltaLocation * Move.SteeringThrow / (TurnRadius * 100);
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	/* Calculate the change in location */
	FVector TranslationDelta = Velocity * 100 * Move.DeltaTime; // multiply by 100 to convert from cm to meters

																/*Apply the change to the transform*/
	FTransform DeltaTransform = FTransform(RotationDelta, TranslationDelta);

	FHitResult HitResult;
	GetOwner()->AddActorWorldTransform(DeltaTransform, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = Velocity * 0.f; // stop the kart if it hits something
	}
}

FVector UGoKartMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * AirDragCoefficient;
}

FVector UGoKartMovementComponent::GetRollingResistance()
{
	float AccelerationDueToGravity = GetWorld()->GetGravityZ() / 100;

	/*
	*AccelerationDueToGravity is a negative number with UE4 defaults.
	*Therefore, no need to multiply the return value by -1
	*/
	return Velocity.GetSafeNormal() * Mass * AccelerationDueToGravity * RollingFrictionCoefficient; //
}

FVector UGoKartMovementComponent::GetVelocity()
{
	return Velocity;
}

float UGoKartMovementComponent::GetMaxThrottle()
{
	return MaxThrottle;
}

void UGoKartMovementComponent::SetVelocity(FVector VelocityIn)
{
	Velocity = VelocityIn;
}

float UGoKartMovementComponent::GetThrottle()
{
	return Throttle;
}

void UGoKartMovementComponent::SetThrottle(float ThrottleIn)
{
	Throttle = ThrottleIn;
}

float UGoKartMovementComponent::GetSteeringThrow()
{
	return SteeringThrow;
}

void UGoKartMovementComponent::SetSteeringThrow(float SteeringThrowIn)
{
	SteeringThrow = SteeringThrowIn;
}