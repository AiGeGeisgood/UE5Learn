// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CustomTickableGameObject.generated.h"

/**
 * 
 */
UCLASS()
class AGGDEMO_API ACustomTickableGameObject : public AActor,public FTickableGameObject
{
	GENERATED_BODY()
public:
	virtual bool IsTickable() const override { return true; }
	virtual bool IsAllowedToTick() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
