// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "XGCollidingPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class XGSAMPLEDEMO_API UXGCollidingPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



};
