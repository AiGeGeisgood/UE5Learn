// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "XGSampleServerHttpType.h"
#include "XGSampleServerRequestType.h"
#include "XGSampleServerResponseType.h"

#include "XGSampleClientBPLibrary.generated.h"



DECLARE_DYNAMIC_DELEGATE_FourParams(FXGCheckVersionDelegate, EXGSampleRequestType, ERequestType, FString, AysncID, bool, bResult, const EXGSampleServerResult&, ResponseType);

DECLARE_DYNAMIC_DELEGATE_FiveParams(FXGLoginDelegate, 
EXGSampleRequestType, ERequestType, 
FString, AysncID,
bool, bResult, 
const EXGSampleServerResult&, ResponseType, 
const FXGLoginRespData&, LoginRespData);

DECLARE_DYNAMIC_DELEGATE_FiveParams(FXGRegisterDelegate,
EXGSampleRequestType, ERequestType,
FString, AysncID,
bool, bResult,
const EXGSampleServerResult&, ResponseType,
const FXGRegisterRespData&, RegisterRespData);


UCLASS()
class XGSAMPLECLIENT_API UXGSampleClientBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ExecuteCheck",
		Keywords = "XGSampleClient Execute Check Version",
		WorldContext = "WorldContextObject"), Category = "XGSampleClient|Common")
	static void XGCheckVersion(const UObject* WorldContextObject, FXGCheckVersionDelegate CheckVersionDelegate);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ExecuteRegister",
		Keywords = "XGSampleClient Execute Register",
		WorldContext = "WorldContextObject"), Category = "XGLoginClient|Simple")
	static void XGRegister(const UObject* WorldContextObject, FXGRegisterReqData RegisterReqData, FXGRegisterDelegate RegisterDelegate);



	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ExecuteLogin",
		Keywords = "XGSampleClient Execute Login",
		WorldContext = "WorldContextObject"), Category = "XGSampleClient|Simple")
	static void XGLogin(const UObject* WorldContextObject, FXGLoginReqData LoginReqData, FXGLoginDelegate LoginDeletgate);



};
