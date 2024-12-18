#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "XGSampleServerHttpType.h"
#include "XGSampleServerRequestType.h"
#include "XGSampleServerResponseType.h"
#include "XGSampleClientBPLibrary.h"

#include "XGSampleClientSubsystem.generated.h"



UCLASS()
class  UXGSampleClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	virtual void Initialize(FSubsystemCollectionBase& Collection)override { Super::Initialize(Collection); }

	virtual void Deinitialize()override { Super::Deinitialize(); }


public:

	void CheckVersion(FXGCheckVersionDelegate  CheckVersionDelegate);

	void Register(FXGRegisterReqData& UserRegisterData, FXGRegisterDelegate RegisterDelegate);

	void Login(FXGLoginReqData& LoginReqData, FXGLoginDelegate LoginDelegate);




protected:


	void SendHttp(EXGSampleRequestType InRequestType, const FString& InRequestDataStr, const FString& InDelegateGuid);

	void OnResponseReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, EXGSampleRequestType RequestType, FString DelegateGuid);

	void ProcessHttpResponse(const FXGSampleServerResponse& LoginServerResponse, EXGSampleRequestType RequestType, const  FString& DelegateGuid);



protected:

	bool LoadXGLoginServerIPAndPort();

	bool LoadMD5TokenAndNum(FString& Token, int32& HashNum);

	FString GenerateToken();

	FString MD5HashPassword(FString InPassword);



protected:

	FString XGLoginServerIP=TEXT("");
	FString XGLoginServerPort=TEXT("");

	TMap<FString, FXGCheckVersionDelegate> CheckVersionDelegatesMap;
	TMap<FString, FXGRegisterDelegate> RegisterDelegates;
	TMap<FString, FXGLoginDelegate> LoginDelegates;

};


