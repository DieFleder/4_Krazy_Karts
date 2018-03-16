// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

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

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	FGoKartMove LastMove;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;
};

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
	void SimulateMove(const FGoKartMove& Move);

	//Mass of the car [kg]
	UPROPERTY(EditAnywhere)
	float Mass = 1000; 

	//Affects the magnitude of air resistance [kg/m]
	UPROPERTY(EditAnywhere)
	float AirDragCoefficient = 16.f;

	//Affects the magnitude of rolling friction
	UPROPERTY(EditAnywhere)
	float RollingFrictionCoefficient = 0.045;

	//Maximum driving force in [N]
	UPROPERTY(EditAnywhere)
	float MaxThrottle = 10000; 

	// Higher Value means less handling [meters]
	UPROPERTY(EditAnywhere)
	float TurnRadius = 0.07f;

	float MaxSpeed = 20.f;

	float Throttle = 0.f;

	float SteeringThrow = 0.f;

	FVector Velocity;

	void MoveForward(float AxisInValue);

	void MoveRight(float AxisInValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	UFUNCTION()
	void OnRep_ServerState();

	void UpdateTransform(FGoKartMove Move);
	
	FVector GetAirResistance();

	FVector GetRollingResistance();

	TArray<FGoKartMove> UnacknowledgedMoves;

	FGoKartMove CreateMove(float DeltaTime);

	void ClearAcknowledgedMoves(FGoKartMove LastMove);
};


