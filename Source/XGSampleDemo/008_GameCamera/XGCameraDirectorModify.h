// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGCameraDirectorModify.generated.h"


USTRUCT()
struct FXGToggleCamerInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "XG")
	AActor* CameraOne = nullptr;

	UPROPERTY(EditAnywhere, Category = "XG")
	float CameraBlendTime = 0.2f;
};


UCLASS()
class XGSAMPLEDEMO_API AXGCameraDirectorModify : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGCameraDirectorModify();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(VisibleAnywhere, Category = "XG")
	TArray<FXGToggleCamerInfo>  CamerInfoList;
	
	float TimeToNextCameraChange = 0.f;

	int32 CamerIndex = -1;
};
