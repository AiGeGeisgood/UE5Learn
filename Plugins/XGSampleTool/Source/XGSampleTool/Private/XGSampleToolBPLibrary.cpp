// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGSampleToolBPLibrary.h"
#include "XGSampleTool.h"

UXGSampleToolBPLibrary::UXGSampleToolBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UXGSampleToolBPLibrary::XGSampleToolSampleFunction(float Param)
{
	return -1;
}

void UXGSampleToolBPLibrary::XGSampleToolSampleFunction2(float& Param)
{
}

//void UXGSampleToolBPLibrary::XGSampleToolSampleFunction3()
//{
//}
//
