// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Mass of the car [kg]
	UPROPERTY(EditAnywhere)
	float Mass = 1000; 

	//Affects the magnitude of air resistance [kg/m]
	UPROPERTY(EditAnywhere)
	float AirDragCoefficient = 16.f;

	//Maximum driving force in [N]
	UPROPERTY(EditAnywhere)
	float MaxThrottle = 10000; 

	// Maximum degrees the kart can rotate per second [degrees/s]
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 70.f;

	float Throttle = 0;

	float MaxSpeed = 20.f;

	float SteeringThrow = 0.f;

	FVector Velocity = FVector(0.f, 0.f, 0.f);

	void MoveForward(float AxisInValue);

	void MoveRight(float AxisInValue);

	void UpdateLocationFromVelocity(float DeltaTime);

	void ApplyRotation(float DeltaTime);
	
	FVector GetResistance();
};
