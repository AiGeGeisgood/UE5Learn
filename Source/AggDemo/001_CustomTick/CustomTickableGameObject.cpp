// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTickableGameObject.h"

void ACustomTickableGameObject::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,TEXT("CustomTickableGameObjectTick is executed"));
}

TStatId ACustomTickableGameObject::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ACustomTickableGameObject, STATGROUP_Tickables);
}
