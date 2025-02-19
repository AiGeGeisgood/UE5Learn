// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CustomTickFunction.generated.h"

/**
 * 
 */

//附加到Level Tick，场景tick才会跟随tick
struct FCustomTickFunc : public FTickFunction
{
	//FTickFunction的核心
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
	
	//virtual FString DiagnosticMessage() override;
	//virtual FName DiagnosticContext(bool bDetailed) override;
};


UCLASS()
class XGSAMPLEDEMO_API ACustomTick01 : public AActor
{
	GENERATED_BODY()

public:
	ACustomTick01();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FCustomTickFunc CustomFuncTick; 
};


UCLASS()
class XGSAMPLEDEMO_API ACustomTick02 : public AActor
{
	GENERATED_BODY()

public:
	ACustomTick02();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FCustomTickFunc CustomFuncTick; 
};

UCLASS()
class XGSAMPLEDEMO_API UCustomTick03 : public UObject
{
	GENERATED_BODY()

public:
	FCustomTickFunc CustomFuncTick; 
};