// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreadProxy.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XGSAMPLEDEMO_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static MyStruct* GetStr()
	{
		if (!StructPtr)
		{
			StructPtr = new MyStruct();
			return StructPtr;
		}
		return StructPtr;
	}


protected:
	void CreateThread();
	void DestroyThread();
	FXGThread* TestThread;
	static MyStruct* StructPtr;

};
