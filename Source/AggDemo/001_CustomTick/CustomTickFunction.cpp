// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTickFunction.h"

void FCustomTickFunc::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,TEXT("CustomFuncTick is executed"));
}

// FString FCustomFuncTick::DiagnosticMessage()
// {
// 	return TEXT("[CustomTick01]");
// }

// FName FCustomFuncTick::DiagnosticContext(bool bDetailed)
// {
// 		return GetParentNativeClass(Target->GetClass())->GetFName();
// }

ACustomTick01::ACustomTick01()
{
}

void ACustomTick01::BeginPlay()
{
	Super::BeginPlay();

	CustomFuncTick.bCanEverTick = true;
	CustomFuncTick.bStartWithTickEnabled = true;
	CustomFuncTick.TickGroup = TG_PostPhysics;

	//最核心的，注册进Level
	CustomFuncTick.RegisterTickFunction(GetLevel());
}

void ACustomTick01::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	CustomFuncTick.UnRegisterTickFunction();
}

ACustomTick02::ACustomTick02()
{
}

void ACustomTick02::BeginPlay()
{
	Super::BeginPlay();
	CustomFuncTick.bCanEverTick = false;
	CustomFuncTick.bStartWithTickEnabled = false;
	CustomFuncTick.TickGroup = TG_PostPhysics;

	//最重要的，注册进Level
	CustomFuncTick.RegisterTickFunction(GetLevel());
}

void ACustomTick02::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	CustomFuncTick.UnRegisterTickFunction();
}