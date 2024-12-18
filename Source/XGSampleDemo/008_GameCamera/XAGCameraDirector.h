// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XAGCameraDirector.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXAGCameraDirector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXAGCameraDirector();





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	UPROPERTY(EditAnywhere, Category = "XG")
	AActor* CameraOne;

	UPROPERTY(EditAnywhere, Category = "XG")
	AActor* CameraTwo;

	float TimeToNextCameraChange = 0.f;




};
