// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGSampleClientBPLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "XGSampleClientSubsystem.h"


UXGSampleClientBPLibrary::UXGSampleClientBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UXGSampleClientBPLibrary::XGCheckVersion(const UObject* WorldContextObject, FXGCheckVersionDelegate CheckVersionDelegate)
{
	check(WorldContextObject);
	if (WorldContextObject)
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleClientSubsystem* LoginSystem = GameInstance->GetSubsystem<UXGSampleClientSubsystem>();

		LoginSystem->CheckVersion(CheckVersionDelegate);
	}
}

void UXGSampleClientBPLibrary::XGRegister(const UObject* WorldContextObject, FXGRegisterReqData RegisterReqData, FXGRegisterDelegate RegisterDelegate)
{
	check(WorldContextObject);
	if (WorldContextObject)
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleClientSubsystem* LoginSystem = GameInstance->GetSubsystem<UXGSampleClientSubsystem>();

		LoginSystem->Register(RegisterReqData, RegisterDelegate);
	}

}

void UXGSampleClientBPLibrary::XGLogin(const UObject* WorldContextObject, FXGLoginReqData LoginReqData, FXGLoginDelegate LoginDeletgate)
{
	check(WorldContextObject);
	if (WorldContextObject)
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleClientSubsystem* LoginSystem = GameInstance->GetSubsystem<UXGSampleClientSubsystem>();

		LoginSystem->Login(LoginReqData, LoginDeletgate);
	}


}
