#pragma once

#include "CoreMinimal.h"

#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGSampleXFSilentBiopsyReqType.h"
#include "XGSampleXFSilentBiopsyRespType.h"


#include "XGSampleXFSilentBiopsyAyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGSampleXFSilentBiopsyDelegate,
FGuid, AsyncID, 
bool, bResult,
FString, Message,
FXGSampleXFSilentBiopsyRespInfo, RespInfo);



UCLASS(meta = (HideThen = true))
class XGSAMPLEXFLINK_API UXGSampleXFSilentBiopsyAyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UXGSampleXFSilentBiopsyAyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGSampleXFSilentBiopsyAyncAction();


public:


	/**
	 * XGSampleXFAyncAction
	 *
	 * @param WorldContextObject			WorldContext
	 *
	 *
	 * @param InAppID						iFlyTek AppID
	 * @param InAPISecret					iFlyTek APISecret
	 * @param InAPIKey						iFlyTek APIKey
	 *
	 * @param InImgDataType					ImgType,Only:[jpg,jpeg,png,bmp]
	 * @param InImgData						ImgBinaryData
	 *
	 *
	 * @return UXGXunFeiSilentBiopsyAsyncAction*		Self ObjectPtr
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XGSampleXFSilentBiopsyAsyncAction",
		Keywords = "XG Sample XunFei Face Recognition FR SilentBiopsy SB"),
		Category = "XGSample|XF|SilentBiopsy")
	static UXGSampleXFSilentBiopsyAyncAction* XGXunFeiSilentBiopsyAsyncAction(UObject* WorldContextObject,
		FString InAppID,
		FString InAPISecret,
		FString InAPIKey,
		EXGSampleXFSilentBiopsyImgTpye InImgDataType,
		const TArray<uint8>& InImgData);


public:

	virtual void Activate() override;

	virtual void Activate_Internal() ;


protected:

	void SendHttp(const FString& InServerURL, const FString& InContentString);

	void OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleXFSilentBiopsyRespInfo RespInfo);

	void CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleXFSilentBiopsyRespInfo RespInfo);

	void RealeaseResources();

protected:

	FString ConvertImgTypeToString(EXGSampleXFSilentBiopsyImgTpye InSilentBiopsyImgType);


public:

	UPROPERTY(BlueprintAssignable)
	FXGSampleXFSilentBiopsyDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGSampleXFSilentBiopsyDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGSampleXFSilentBiopsyDelegate OnFail;

protected:

	FGuid AsyncID = FGuid::NewGuid();

	FString AppID = TEXT("");

	FString APISecret = TEXT("");

	FString APIKey = TEXT("");

	EXGSampleXFSilentBiopsyImgTpye ImgType = EXGSampleXFSilentBiopsyImgTpye::Jpeg;

	TArray<uint8> ImgBinaryData;

	FXGSampleXFSilentBiopsyReqInfo ReqInfo;


};