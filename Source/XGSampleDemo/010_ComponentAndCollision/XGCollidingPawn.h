// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "XGCollidingPawn.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGCollidingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AXGCollidingPawn();


	virtual UPawnMovementComponent* GetMovementComponent() const override;




protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem;

	UPROPERTY()
	class UXGCollidingPawnMovementComponent* OurMovementComponent;


	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void Turn(float AxisValue);

	void ParticleToggle();







public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
