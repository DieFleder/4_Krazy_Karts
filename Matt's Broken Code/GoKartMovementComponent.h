// Fill out your copyright notice in the Description page of Project Settings.
/*
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"


USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		float Throttle;

	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;

	UPROPERTY()
		double Time;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public: /* ~~PROPERTIES~~ /


public: /* ~~METHODS~~ /
	// Sets default values for this component's properties
	UGoKartMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FGoKartMove CreateMove(float DeltaTime);

	void SimulateMove(const FGoKartMove& Move);

	void UpdateTransform(FGoKartMove Move);

	FVector GetAirResistance();

	FVector GetRollingResistance();

	FVector GetVelocity();

	void SetVelocity(FVector VelocityIn);

	float GetMaxThrottle();

	float GetThrottle();

	void SetThrottle(float ThrottleIn);
	 
	float GetSteeringThrow();

	void SetSteeringThrow(float SteeringThrowIn);


protected: /* ~~PROPERTIES~~ /


protected: /* ~~METHODS~~ /
	// Called when the game starts
	virtual void BeginPlay() override;


private: /* ~~PROPERTIES~~ /
	//Affects the magnitude of air resistance [kg/m]
	UPROPERTY(EditAnywhere)
	float AirDragCoefficient = 16.f;

	//Affects the magnitude of rolling friction
	UPROPERTY(EditAnywhere)
	float RollingFrictionCoefficient = 0.045;

	//Maximum driving force in [N]
	UPROPERTY(EditAnywhere)
	float MaxThrottle = 10000;

	//Mass of the car [kg]
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Higher Value means less handling [meters]
	UPROPERTY(EditAnywhere)
	float TurnRadius = 0.07f;

	FVector Velocity;

	float Throttle = 0.f;

	float SteeringThrow = 0.f;

private: /* ~~METHODS~~ /

};
*/