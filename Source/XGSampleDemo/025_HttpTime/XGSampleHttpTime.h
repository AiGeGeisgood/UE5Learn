

#pragma once

#include "CoreMinimal.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"



#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGSampleHttpTime.generated.h"

//https://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp
//https://acs.m.taobao.com/gw/mtop.common.getTimestamp/
//http://47.108.203.10:8036/NetTime

/**
 * EXGSampleNetTimeType
 * ToDo More URL
 */
UENUM(BlueprintType)
enum class EXGSampleNetTimeType :uint8
{
	Local UMETA(DisplayName = "LocalTimeAPI"),
	TaoBao UMETA(DisplayName = "TaobaoTimeAPI"),
	XGServer UMETA(DisplayName = "XGServerTimeAPI"),
	MaxNum UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct XGSAMPLEDEMO_API FXGSampleNetTimeRespDataInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FString t = TEXT("");

};

USTRUCT(BlueprintType)
struct XGSAMPLEDEMO_API FXGSampleNetTimeRespMessage
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FString api = TEXT("");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FString v = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	TArray<FString> ret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FXGSampleNetTimeRespDataInfo data;

};

/**
 * FXGSampleNetTimeRespInfo
 *
 */
USTRUCT(BlueprintType)
struct XGSAMPLEDEMO_API FXGSampleNetTimeRespInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FDateTime BeijingDateTime = FDateTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FDateTime UTCDateTime = FDateTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FXGSampleNetTimeRespMessage RespMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGNetTime")
	FString RawMessage =TEXT("");

};




DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGSampleNetTimeDelegate, FGuid, AsyncID, bool, bResult, FString, Message, FXGSampleNetTimeRespInfo, RespInfo);

/**
 * 
 */
UCLASS(meta = (HideThen = true))
class XGSAMPLEDEMO_API UXGSampleHttpTimeAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UXGSampleHttpTimeAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGSampleHttpTimeAsyncAction();


	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XGSampleHttpTimeAsyncAction",
		Keywords = "XG Sample Net Time"),
		Category = "XGSample|NetTime")
	static UXGSampleHttpTimeAsyncAction* XGSampleHttpTimeAsyncAction(UObject* WorldContextObject,
		EXGSampleNetTimeType InNetTimeType);

public:

	virtual void Activate() override;

	virtual void Activate_Internal();

public:

	UPROPERTY(BlueprintAssignable)
	FXGSampleNetTimeDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGSampleNetTimeDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGSampleNetTimeDelegate OnFail;

protected:

	void SendHttp(const FString& InServerURL);

	void OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGuid InAsyncID);

	void CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleNetTimeRespInfo PRRespInfo);

	void CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleNetTimeRespInfo PRRespInfo);

	void RealeaseResources();


protected:


	FGuid AsyncID = FGuid::NewGuid();

	EXGSampleNetTimeType NetTimeType = EXGSampleNetTimeType::Local;



};
