#pragma once

#include "CoreMinimal.h"

#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGSampleBDReqType.h"
#include "XGSampleBDRespType.h"


#include "XGSampleBPAyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGSampleBDLinkDelegate,
FGuid, AsyncID, 
bool, bResult,
FString, Message,
FXGSampleBDRespInfo, RespInfo);



UCLASS(meta = (HideThen = true))
class XGSAMPLEBDLINK_API UXGSampleBDAyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UXGSampleBDAyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGSampleBDAyncAction();

public:


	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "BDChat-ERNIE-Tiny-8K",
		Keywords = "BD BaiDu Chat ERNIE Tiny 4.0 8K"),
		Category = "XGSampleLink|BDLink")
	static UXGSampleBDAyncAction* XGBDChat(UObject* WorldContextObject,
		FString InSecretKeyNotAppSecretKey,
		FString InAccessKeyNotAppAPIKey,
		FXGSampleBDReqInfo InReqInfo);



public:

	virtual void Activate() override;

	virtual void Activate_Internal() ;

protected:

	void SendHttp(const FString& InServerURL, const FString& InContentString, TMap<FString, FString>InHeaders);

	void OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool HttpSream(void* Ptr, int64 Length);

	void HandleStreamData(TArray<uint8> InStreamData);

	void RecursiveProcessingStreamData();


	void CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleBDRespInfo RespInfo);

	void CallOnUpdate(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleBDRespInfo RespInfo);

	void CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleBDRespInfo RespInfo);

	void RealeaseResources();


	UPROPERTY(BlueprintAssignable)
	FXGSampleBDLinkDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGSampleBDLinkDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGSampleBDLinkDelegate OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FXGSampleBDLinkDelegate OnFail;

protected:

	FGuid AsyncID = FGuid::NewGuid();

	FString APISecret = TEXT("");

	FString APIKey = TEXT("");

	FXGSampleBDReqInfo ReqInfo;

	FString TotalString = TEXT("");

	TArray<uint8> ErrorSteamData;

	TArray<uint8> StreamData;



};