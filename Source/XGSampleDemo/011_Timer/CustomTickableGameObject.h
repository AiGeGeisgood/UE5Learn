// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CustomTickableGameObject.generated.h"

/**
 * 
 */

//TickGameObject跟主进程tick有关，只要对象存在，会一直tick
UCLASS()
class XGSAMPLEDEMO_API ACustomTickableGameObject : public AActor, public FTickableGameObject
{
	GENERATED_BODY()

public:
	ACustomTickableGameObject():
		VIsTickable(true),
		VIsAllowedToTick(true),
		VTickableWhenPaused(true),
		VTickableInEditor(true)
	{
	}

	virtual bool IsTickable() const override { return VIsTickable; }
	virtual bool IsAllowedToTick() const override { return VIsAllowedToTick; }
	virtual bool IsTickableWhenPaused() const override { return VTickableWhenPaused; }
	virtual bool IsTickableInEditor() const override { return VTickableInEditor; }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	UPROPERTY(EditAnywhere, Blueprintable)
	uint8 VIsTickable : 1;
	UPROPERTY(EditAnywhere, Blueprintable)
	uint8 VIsAllowedToTick : 1;
	UPROPERTY(EditAnywhere, Blueprintable)
	uint8 VTickableWhenPaused : 1;
	UPROPERTY(EditAnywhere, Blueprintable)
	uint8 VTickableInEditor : 1;
};
