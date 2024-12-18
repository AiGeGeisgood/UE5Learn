


#include "XGSampleHttpTime.h"

#include "JsonObjectConverter.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"



DEFINE_LOG_CATEGORY_STATIC(LogXGSampleNetTime,Display,All);


UXGSampleHttpTimeAsyncAction::UXGSampleHttpTimeAsyncAction(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{


}

UXGSampleHttpTimeAsyncAction::~UXGSampleHttpTimeAsyncAction()
{
	RealeaseResources();

}

UXGSampleHttpTimeAsyncAction* UXGSampleHttpTimeAsyncAction::XGSampleHttpTimeAsyncAction(UObject* WorldContextObject, EXGSampleNetTimeType InNetTimeType)
{

	UXGSampleHttpTimeAsyncAction* AsyncAction = NewObject<UXGSampleHttpTimeAsyncAction>();

	AsyncAction->NetTimeType = InNetTimeType;

	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGSampleHttpTimeAsyncAction::Activate()
{
	Super::Activate();


	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});


	FXGSampleNetTimeRespInfo RespInfo;

	Then.Broadcast(AsyncID, false, TEXT("UXGSampleHttpTimeAsyncAction is just started,please wait to be finished!"), RespInfo);



}

void UXGSampleHttpTimeAsyncAction::Activate_Internal()
{

	UE_LOG(LogXGSampleNetTime, Display, TEXT("[%s],AsyncID:[%s],Begin to execute NetTimeAsyncAction"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString TimeServerURL = TEXT("");

	switch (NetTimeType)
	{
	case EXGSampleNetTimeType::Local:
		break;
	case EXGSampleNetTimeType::TaoBao:
		TimeServerURL = TEXT("https://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp");
		break;

	case EXGSampleNetTimeType::XGServer:
		TimeServerURL = TEXT("http://47.108.203.10:8036/NetTime");
		break;
	case EXGSampleNetTimeType::MaxNum:
		break;
	default:
		break;
	}

	if (!TimeServerURL.IsEmpty())
	{
		SendHttp(TimeServerURL);

		return;
	}

	FXGSampleNetTimeRespInfo RespInfo;


	RespInfo.BeijingDateTime = FDateTime::Now();
	RespInfo.UTCDateTime = FDateTime::UtcNow();



	CallOnSuccess(AsyncID,true,TEXT("这是本地时间"), RespInfo);
	RealeaseResources();


}

void UXGSampleHttpTimeAsyncAction::SendHttp(const FString& InServerURL)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UXGSampleHttpTimeAsyncAction::OnHttpRespReceived, AsyncID);

	Request->SetURL(InServerURL);

	Request->SetVerb("Post");

	Request->SetHeader("XGGuid", AsyncID.ToString());

	Request->SetHeader("Content-Type", "application/json");

	Request->SetContentAsString(TEXT(""));

	Request->ProcessRequest();



}

void UXGSampleHttpTimeAsyncAction::OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded,FGuid InAsyncID)
{

	UE_LOG(LogXGSampleNetTime, Display, TEXT("[%s],AsyncID:[%s],Receive NetTimeAsyncAction HttpResonpse"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString Back1AsyncID = InAsyncID.ToString();

	FString Back2AsyncID  =HttpRequest->GetHeader(TEXT("XGGuid"));

	FXGSampleNetTimeRespInfo RespInfo;

	//有时候HttpResponse是空,是因为URL写错了

	if (bSucceeded && HttpRequest->GetStatus() == EHttpRequestStatus::Succeeded && HttpResponse->GetResponseCode() == 200)
	{
		FString ResponseJson = HttpResponse->GetContentAsString();

		UE_LOG(LogXGSampleNetTime, Display, TEXT("[%s]"), *ResponseJson);

		FXGSampleNetTimeRespMessage RespMessage;

		bool bParseJson = FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &RespMessage);

		if (bParseJson)
		{
			FString TickString = RespMessage.data.t;

			int64 TickTime = FCString::Atoi64(*TickString) / 1000;

			RespInfo.UTCDateTime = FDateTime::FromUnixTimestamp(TickTime);

			RespInfo.BeijingDateTime = FDateTime::FromUnixTimestamp(TickTime + 8 * 60 * 60);

			RespInfo.RawMessage = ResponseJson;

			RespInfo.RespMessage = RespMessage;

			CallOnSuccess(AsyncID, true, TEXT("Succeed!"), RespInfo);


		}
		else
		{
			FString ErrorMessage = FString::Printf(TEXT("[%s],AsyncID:[%s],Receive NetTimeAsyncAction HttpResonpse Parse Json Wrong!, Check Your Net or Connect The Author"),
				*FString(__FUNCTION__), *(AsyncID.ToString()));


			UE_LOG(LogXGSampleNetTime, Error, TEXT("[%s]"), *ErrorMessage);

			CallOnFail(AsyncID, false, ErrorMessage, RespInfo);

		}




	}
	else
	{
		FString ErrorMessage = FString::Printf(TEXT("[%s],AsyncID:[%s],Receive NetTimeAsyncAction HttpResonpse Failed, Check Your Net"),
		*FString(__FUNCTION__), *(AsyncID.ToString()));


		UE_LOG(LogXGSampleNetTime, Error, TEXT("[%s]"), *ErrorMessage);

		CallOnFail(AsyncID, false, ErrorMessage, RespInfo);
	}







	RealeaseResources();

}

void UXGSampleHttpTimeAsyncAction::CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleNetTimeRespInfo RespInfo)
{
	FXGSampleNetTimeDelegate TempDelegate = OnSuccess;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);

		});

}

void UXGSampleHttpTimeAsyncAction::CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleNetTimeRespInfo RespInfo)
{

	FXGSampleNetTimeDelegate TempDelegate = OnFail;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);

		});
}

void UXGSampleHttpTimeAsyncAction::RealeaseResources()
{
	
	 Then.Clear();
	 OnSuccess.Clear();
	 OnFail.Clear();

	SetReadyToDestroy();
}
