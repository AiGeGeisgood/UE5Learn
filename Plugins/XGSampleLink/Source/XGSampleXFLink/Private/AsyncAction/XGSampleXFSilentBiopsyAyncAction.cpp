#include "XGSampleXFSilentBiopsyAyncAction.h"

#include "JsonObjectConverter.h"
#include "Containers/StringConv.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Base64.h"
#include "Runtime/Launch/Resources/Version.h"

#include "XGSampleXFLinkBase.h"
#include "LogXGXF.h"

UXGSampleXFSilentBiopsyAyncAction::UXGSampleXFSilentBiopsyAyncAction(const FObjectInitializer& ObjectInitializer)
{
}

UXGSampleXFSilentBiopsyAyncAction::~UXGSampleXFSilentBiopsyAyncAction()
{
	RealeaseResources();
}

UXGSampleXFSilentBiopsyAyncAction* UXGSampleXFSilentBiopsyAyncAction::XGXunFeiSilentBiopsyAsyncAction(UObject* WorldContextObject,
FString InAppID, FString InAPISecret, FString InAPIKey, EXGSampleXFSilentBiopsyImgTpye InImgDataType, const TArray<uint8>& InImgData)
{
	UXGSampleXFSilentBiopsyAyncAction* AsyncAction = NewObject<UXGSampleXFSilentBiopsyAyncAction>();

	AsyncAction->AppID = InAppID;
	AsyncAction->APISecret = InAPISecret;
	AsyncAction->APIKey = InAPIKey;

	AsyncAction->ImgType = InImgDataType;
	AsyncAction->ImgBinaryData = InImgData;

	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGSampleXFSilentBiopsyAyncAction::Activate()
{
	Super::Activate();

	FXGSampleXFSilentBiopsyRespInfo RespInfo;

	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});


	Then.Broadcast(AsyncID, false, TEXT("SilentBiopsy is just started,please wait to be finished!"), RespInfo);




}

void UXGSampleXFSilentBiopsyAyncAction::Activate_Internal()
{
	UE_LOG(LogXGXF, Display, TEXT("[%s],AsyncID:[%s],Begin to execute SilentBiopsy AsyncAction"), *FString(__FUNCTION__), *(AsyncID.ToString()));


	FString iFlyTekURL = TEXT("http://api.xf-yun.com/v1/private/s67c9c78c");

	FString AuthURL = TEXT("");

	FString Protocol = TEXT("");

	FXGSampleXFLinkBase::AssembleAuthUrl(iFlyTekURL, APISecret, APIKey, AuthURL, Protocol);

	ReqInfo.header.app_id = AppID;

	ReqInfo.payload.input1.encoding= ConvertImgTypeToString(ImgType);

	FString ImgBase64String =FBase64::Encode(ImgBinaryData);

	uint64 FileMaxSize = 4 * 1024 * 1024;

	if (ImgBase64String.IsEmpty() || ImgBase64String.GetAllocatedSize() > FileMaxSize)
	{
		CallOnFail(AsyncID, false, 
		TEXT("Img_1 Size is not right ,please input right size Img and the size after encoding must be less than 4MB"), 
		FXGSampleXFSilentBiopsyRespInfo());

		RealeaseResources();
		return;
	}

	ReqInfo.payload.input1.image = ImgBase64String;

	FString ContentString = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(ReqInfo, ContentString);


	SendHttp(AuthURL, ContentString);

}

void UXGSampleXFSilentBiopsyAyncAction::SendHttp(const FString& InServerURL, const FString& InContentString)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UXGSampleXFSilentBiopsyAyncAction::OnHttpRespReceived);

	Request->SetURL(InServerURL);

	Request->SetVerb("Post");

	Request->SetHeader("Content-Type", "application/json");

	Request->SetContentAsString(InContentString);

	Request->ProcessRequest();


}

void UXGSampleXFSilentBiopsyAyncAction::OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogXGXF, Display, TEXT("[%s],AsyncID:[%s],Receive SilentBiopsy HttpResonpse"), *FString(__FUNCTION__), *(AsyncID.ToString()));
	FString ResponseJson = HttpResponse->GetContentAsString();
	UE_LOG(LogXGXF, Display, TEXT("[%s],[%s]"), *FString(__FUNCTION__), *ResponseJson);


	if (bSucceeded && HttpRequest->GetStatus() == EHttpRequestStatus::Succeeded && HttpResponse->GetResponseCode() == 200)
	{
		FXGSampleXFSilentBiopsyRespMessage RespMessage;
		FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &RespMessage);

		if (RespMessage.header.code == 0)
		{

			FString TextString = FXGSampleXFLinkBase::Base64UTF8StringToTCharString(RespMessage.payload.anti_spoof_result.text);

			UE_LOG(LogXGXF, Display, TEXT("[%s],TextString:[%s]"), *FString(__FUNCTION__), *TextString);


			FXGSampleXFSilentBiopsyRespTextMessage RespTextMessage;

			bool bParseSilentBiopsyRespPagesMessage = FJsonObjectConverter::JsonObjectStringToUStruct(TextString, &RespTextMessage);


			if (!bParseSilentBiopsyRespPagesMessage)
			{

				UE_LOG(LogXGXF, Error, TEXT("[%s],AsyncID:[%s],bParseSilentBiopsyRespTextMessage is false"), *FString(__FUNCTION__), *(AsyncID.ToString()));

				FXGSampleXFSilentBiopsyRespInfo RespInfo;
				RespInfo.RawMessage = RespMessage;

				CallOnFail(AsyncID, false, TEXT("Parse Json Failed!"), RespInfo);

			}
			else if(RespTextMessage.ret ==0)
			{

				FXGSampleXFSilentBiopsyRespInfo RespInfo;

				RespInfo.bPassed = RespTextMessage.passed;

				RespInfo.Score = RespTextMessage.score;

				RespInfo.TextMessage = RespTextMessage;

				RespInfo.RawMessage = RespMessage;

				CallOnSuccess(AsyncID, true, TEXT("SilentBiopsy has finished !"), RespInfo);

			}
			else
			{

				UE_LOG(LogXGXF, Warning, TEXT("[%s],AsyncID:[%s],bParseSilentBiopsyRespTextMessage is false,Error code is [%d]"), *FString(__FUNCTION__), *(AsyncID.ToString()), RespTextMessage.ret);

				FXGSampleXFSilentBiopsyRespInfo RespInfo;

				RespInfo.bPassed = RespTextMessage.passed;

				RespInfo.Score = RespTextMessage.score;

				RespInfo.TextMessage = RespTextMessage;

				RespInfo.RawMessage = RespMessage;

				CallOnFail(AsyncID, false, 
				TEXT("SilentBiopsy TextMessage has Error ! Please check error code !Error Code is :") + FString::FromInt(RespTextMessage.ret), 
				RespInfo);


			}





		}
		else
		{

			FXGSampleXFSilentBiopsyRespInfo RespInfo;
			RespInfo.RawMessage = RespMessage;


			CallOnFail(AsyncID, false, RespMessage.header.message, RespInfo);

		}





	}
	else
	{
		FString ErrorResponseJson = HttpResponse->GetContentAsString();

		FXGSampleXFSilentBiopsyRespInfo RespInfo;

		RespInfo.RawMessage.header.code = HttpResponse->GetResponseCode();

		RespInfo.RawMessage.header.message = ErrorResponseJson;

		CallOnFail(AsyncID, false, ErrorResponseJson, RespInfo);
	}




	RealeaseResources();


}

void UXGSampleXFSilentBiopsyAyncAction::CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleXFSilentBiopsyRespInfo RespInfo)
{
	FXGSampleXFSilentBiopsyDelegate TempDelegate = OnSuccess;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);

		});
}

void UXGSampleXFSilentBiopsyAyncAction::CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleXFSilentBiopsyRespInfo RespInfo)
{
	FXGSampleXFSilentBiopsyDelegate TempDelegate = OnFail;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);

		});
}

void UXGSampleXFSilentBiopsyAyncAction::RealeaseResources()
{
	Then.Clear();

	OnSuccess.Clear();

	OnFail.Clear();

	AsyncID = FGuid();

	AppID = TEXT("");

	APISecret = TEXT("");

	APIKey = TEXT("");

	ImgType = EXGSampleXFSilentBiopsyImgTpye::Jpeg;

	ImgBinaryData.Empty();

	ReqInfo = FXGSampleXFSilentBiopsyReqInfo();

	SetReadyToDestroy();

}

FString UXGSampleXFSilentBiopsyAyncAction::ConvertImgTypeToString(EXGSampleXFSilentBiopsyImgTpye InSilentBiopsyImgType)
{

	FString ImgStr = TEXT("");
	switch (InSilentBiopsyImgType)
	{
	case EXGSampleXFSilentBiopsyImgTpye::Jpg:
		ImgStr = TEXT("jpg");
		break;
	case EXGSampleXFSilentBiopsyImgTpye::Jpeg:
		ImgStr = TEXT("jpeg");
		break;
	case EXGSampleXFSilentBiopsyImgTpye::Png:
		ImgStr = TEXT("png");
		break;
	case EXGSampleXFSilentBiopsyImgTpye::Bmp:
		ImgStr = TEXT("bmp");
		break;
	default:
		ImgStr = TEXT("jpg");
		break;
	}

	return ImgStr;
}
