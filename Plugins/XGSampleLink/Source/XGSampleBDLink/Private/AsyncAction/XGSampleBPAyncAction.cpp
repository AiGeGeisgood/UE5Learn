#include "XGSampleBPAyncAction.h"

#include "JsonObjectConverter.h"
#include "Containers/StringConv.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Base64.h"
#include "Runtime/Launch/Resources/Version.h"

#include "XGSampleXFLinkBase.h"
#include "LogXGBD.h"

UXGSampleBDAyncAction::UXGSampleBDAyncAction(const FObjectInitializer& ObjectInitializer)
{
}

UXGSampleBDAyncAction::~UXGSampleBDAyncAction()
{
	RealeaseResources();
}

UXGSampleBDAyncAction* UXGSampleBDAyncAction::XGBDChat(UObject* WorldContextObject,
FString InSecretKeyNotAppSecretKey,
FString InAccessKeyNotAppAPIKey,
FXGSampleBDReqInfo InReqInfo)
{
	UXGSampleBDAyncAction* AsyncAction = NewObject<UXGSampleBDAyncAction>();

	AsyncAction->APIKey = InAccessKeyNotAppAPIKey;
	AsyncAction->APISecret = InSecretKeyNotAppSecretKey;

	AsyncAction->ReqInfo = InReqInfo;


	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGSampleBDAyncAction::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});

	Then.Broadcast(AsyncID, false, TEXT("XGSampleBDAyncAction is just started,please wait to be finished!"),
	FXGSampleBDRespInfo());


}

void UXGSampleBDAyncAction::Activate_Internal()
{

	UE_LOG(LogXGBD, Display, TEXT("[%s],AsyncID:[%s],Begin to execute BD Chat AsyncAction"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString BaiDuURL = TEXT("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/ernie-lite-8k");

	TMap<FString, FString> Headers =FXGSampleXFLinkBase::GenerateBDHeaders(BaiDuURL, APIKey, APISecret);;

	FString ConentString = ReqInfo.ToJsonString();

	UE_LOG(LogXGBD, Display, TEXT("[%s],[%s]"), *FString(__FUNCTION__), *ConentString);

	SendHttp(BaiDuURL, ConentString, Headers);

}

void UXGSampleBDAyncAction::SendHttp(const FString& InServerURL, const FString& InContentString, TMap<FString, FString> InHeaders)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UXGSampleBDAyncAction::OnHttpRespReceived);

	FHttpRequestStreamDelegate Delegate;
	Delegate.BindUObject(this, &UXGSampleBDAyncAction::HttpSream);

	if (ReqInfo.stream)
	{
		Request->SetResponseBodyReceiveStreamDelegate(Delegate);
	}


	Request->SetURL(InServerURL);


	for (auto& Tmp : InHeaders)
	{
		Request->SetHeader(Tmp.Key, Tmp.Value);

	}

	Request->SetVerb("Post");

	Request->SetContentAsString(InContentString);

	Request->ProcessRequest();
}

void UXGSampleBDAyncAction::OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogXGBD, Display, TEXT("[%s],AsyncID:[%s],Receive BD Chat HttpResonpse"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString ResponseJson = HttpResponse->GetContentAsString();

	UE_LOG(LogXGBD, Display, TEXT("[%s],[%s]"), *FString(__FUNCTION__), *ResponseJson);

	if (bSucceeded && HttpRequest->GetStatus() == EHttpRequestStatus::Succeeded && HttpResponse->GetResponseCode() == 200)
	{
		if (ReqInfo.stream)
		{
			if (ErrorSteamData.Num() > 0)
			{

				FUTF8ToTCHAR ErrorJsonString((const ANSICHAR*)ErrorSteamData.GetData(), ErrorSteamData.Num());
				FString ErrorString(ErrorJsonString.Length(), ErrorJsonString.Get());
				TSharedPtr<FJsonObject> ErrorJsonObject;
				TSharedRef<TJsonReader<TCHAR>>TextJsonReader = TJsonReaderFactory<TCHAR>::Create(ErrorString);
				bool IsDeserialize = FJsonSerializer::Deserialize(TextJsonReader, ErrorJsonObject);
				if (!IsDeserialize)
				{
					UE_LOG(LogXGBD, Error, TEXT("[%s],ParseJson Failed!!!!!,JsonStr:[%s]"), *FString(__FUNCTION__), *ErrorString);
					FXGSampleBDRespInfo ErrorRespInfo;
					ErrorRespInfo.bReqStream = ReqInfo.stream;
					CallOnFail(AsyncID, false, TEXT("ParseJson Failed!!!!!"), ErrorRespInfo);
				}
				else
				{
					FXGSampleRespErrorInfo ErrorInfo;
					FJsonObjectConverter::JsonObjectStringToUStruct(ErrorString, &ErrorInfo);

					FXGSampleBDRespInfo RespInfo;

					RespInfo.bResult = false;
					RespInfo.bReqStream = true;
					RespInfo.Message = ErrorInfo.error_msg;
					RespInfo.ErrorMessage = ErrorInfo;

					RespInfo.RawMessage = ErrorString;



					UE_LOG(LogXGBD, Warning, TEXT("[%s],Error Stream RespInfo,JsonStr:[%s]"), *FString(__FUNCTION__), *ErrorString);
					CallOnFail(AsyncID, false, RespInfo.Message, RespInfo);

				}

			}
			else
			{
				FXGSampleRespSteamFinalMessage SteamFinalMessage;


				FString XRatelimitLimitRequestsStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Limit-Requests"));
				if (!XRatelimitLimitRequestsStr.IsEmpty())
				{
					SteamFinalMessage.HeaderInfo.XRatelimitLimitRequests = FCString::Atoi(*XRatelimitLimitRequestsStr);
				}

				FString XRatelimitLimitTokensStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Limit-Tokens"));
				if (!XRatelimitLimitTokensStr.IsEmpty())
				{
					SteamFinalMessage.HeaderInfo.XRatelimitLimitTokens = FCString::Atoi(*XRatelimitLimitTokensStr);
				}

				FString XRatelimitRemainingRequestsStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Remaining-Requests"));
				if (!XRatelimitRemainingRequestsStr.IsEmpty())
				{
					SteamFinalMessage.HeaderInfo.XRatelimitRemainingRequests = FCString::Atoi(*XRatelimitRemainingRequestsStr);
				}

				FString XRatelimitRemainingTokensStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Remaining-Tokens"));
				if (!XRatelimitRemainingTokensStr.IsEmpty())
				{
					SteamFinalMessage.HeaderInfo.XRatelimitRemainingTokens = FCString::Atoi(*XRatelimitRemainingTokensStr);
				}

				SteamFinalMessage.TotalString = TotalString;

				FXGSampleBDRespInfo RespInfo;
				RespInfo.bResult = true;
				RespInfo.bReqStream = true;
				RespInfo.SteamFinalMessage = SteamFinalMessage;
				RespInfo.Message = TotalString;


				CallOnSuccess(AsyncID, true, RespInfo.Message, RespInfo);



			}



		}
		else 
		{

			TSharedPtr<FJsonObject> TextJsonObject;
			TSharedRef<TJsonReader<TCHAR>>TextJsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseJson);
			bool IsDeserialize = FJsonSerializer::Deserialize(TextJsonReader, TextJsonObject);

			if (!IsDeserialize)
			{

				UE_LOG(LogXGBD, Error, TEXT("[%s],ParseJson Failed!!!!!,JsonStr:[%s]"), *FString(__FUNCTION__), *ResponseJson);

				FXGSampleBDRespInfo HttpError;
				HttpError.bReqStream = ReqInfo.stream;

				CallOnFail(AsyncID, false, TEXT("ParseJson Failed!!!!!"), HttpError);
			}
			else
			{

				bool bError = TextJsonObject->TryGetField(TEXT("error_code")) || TextJsonObject->TryGetField(TEXT("error_msg"));

				if (bError)
				{
					FXGSampleRespErrorInfo ErrorInfo;
					FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &ErrorInfo);

					FXGSampleBDRespInfo RespInfo;
					RespInfo.bReqStream = ReqInfo.stream;

					RespInfo.bResult = false;
					RespInfo.Message = ErrorInfo.error_msg;
					RespInfo.ErrorMessage = ErrorInfo;
					RespInfo.RawMessage = ResponseJson;


					UE_LOG(LogXGBD, Warning, TEXT("[%s],Error No Stream RespInfo,JsonStr:[%s]"), *FString(__FUNCTION__), *ResponseJson);

					CallOnFail(AsyncID, false, RespInfo.Message, RespInfo);


				}
				else
				{
					FXGSampleRespNoSteamFinalMessage NoSteamFinalMessage;
					FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &NoSteamFinalMessage);

					FString XRatelimitLimitRequestsStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Limit-Requests"));
					if (!XRatelimitLimitRequestsStr.IsEmpty())
					{
						NoSteamFinalMessage.HeaderInfo.XRatelimitLimitRequests = FCString::Atoi(*XRatelimitLimitRequestsStr);
					}

					FString XRatelimitLimitTokensStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Limit-Tokens"));
					if (!XRatelimitLimitTokensStr.IsEmpty())
					{
						NoSteamFinalMessage.HeaderInfo.XRatelimitLimitTokens = FCString::Atoi(*XRatelimitLimitTokensStr);
					}

					FString XRatelimitRemainingRequestsStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Remaining-Requests"));
					if (!XRatelimitRemainingRequestsStr.IsEmpty())
					{
						NoSteamFinalMessage.HeaderInfo.XRatelimitRemainingRequests = FCString::Atoi(*XRatelimitRemainingRequestsStr);
					}

					FString XRatelimitRemainingTokensStr = HttpResponse->GetHeader(TEXT("X-Ratelimit-Remaining-Tokens"));
					if (!XRatelimitRemainingTokensStr.IsEmpty())
					{
						NoSteamFinalMessage.HeaderInfo.XRatelimitRemainingTokens = FCString::Atoi(*XRatelimitRemainingTokensStr);
					}

					FXGSampleBDRespInfo RespInfo;
					RespInfo.bReqStream = ReqInfo.stream;

					RespInfo.NoSteamFinalMessage = NoSteamFinalMessage;
					RespInfo.bResult = true;
					RespInfo.Message = NoSteamFinalMessage.result;
					RespInfo.RawMessage = ResponseJson;

					CallOnSuccess(AsyncID, true, RespInfo.Message, RespInfo);

				}

			}
		}

	}
	else
	{
		FXGSampleBDRespInfo HttpError;

		HttpError.bReqStream = ReqInfo.stream;

		UE_LOG(LogXGBD, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ResponseJson);

		CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ResponseJson, HttpError);
	}





	RealeaseResources();

}

bool UXGSampleBDAyncAction::HttpSream(void* Ptr, int64 Length)
{
	TArray<uint8> TempStreamData;

	TempStreamData.Append((uint8*)Ptr, Length);

	UXGSampleBDAyncAction* TestPtr = this;

	if (TestPtr)
	{
		AsyncTask(ENamedThreads::GameThread, [TempStreamData, TestPtr]() {

			if (TestPtr)
			{
				TestPtr->HandleStreamData(TempStreamData);
			}


		});

	}

	return true;
}

void UXGSampleBDAyncAction::HandleStreamData(TArray<uint8> InStreamData)
{
	if (IsEngineExitRequested())
	{
		return;
	}

	FUTF8ToTCHAR UTF8String((const ANSICHAR*)InStreamData.GetData(), InStreamData.Num());

	FString OriginIndexedJson(UTF8String.Length(), UTF8String.Get());

	TSharedPtr<FJsonObject> TextJsonObject;
	TSharedRef<TJsonReader<TCHAR>>TextJsonReader = TJsonReaderFactory<TCHAR>::Create(OriginIndexedJson);

	bool IsDeserialize = FJsonSerializer::Deserialize(TextJsonReader, TextJsonObject);

	if (IsDeserialize && TextJsonObject->TryGetField(TEXT("error_code")))
	{
		ErrorSteamData = InStreamData;
		return;
	}

	StreamData.Append(InStreamData);

	RecursiveProcessingStreamData();



}

void UXGSampleBDAyncAction::RecursiveProcessingStreamData()
{

	int32 RecoredIndex = -1;

	for (size_t Index = 0; Index < StreamData.Num(); Index++)
	{
		if (StreamData[Index] == 10 && Index + 1 < StreamData.Num() && StreamData[Index + 1] == 10)
		{
			RecoredIndex = Index;
			break;
		}
	}
	if (RecoredIndex != -1)
	{
	
		TArray<uint8> HandleStreamData;
		HandleStreamData.Append(StreamData.GetData(), RecoredIndex);

		StreamData.RemoveAt(0, RecoredIndex + 2);

		FUTF8ToTCHAR HandleString((const ANSICHAR*)HandleStreamData.GetData(), HandleStreamData.Num());

		FString OriginHandleJson(HandleString.Length(), HandleString.Get());

		FString HandleJson(HandleString.Length(), HandleString.Get());

		HandleJson.RemoveFromStart(TEXT("data:"));

		UE_LOG(LogXGBD, Display, TEXT("[%s], Stream  data Apeend ,JsonStr:[%s]"), *FString(__FUNCTION__), *OriginHandleJson);

		if (HandleJson.Contains(TEXT("\n")))
		{
			UE_LOG(LogXGBD, Error, TEXT("[%s],Fatal Error  Stream  data Apeend Wroing!!!,JsonStr:[%s]"), *FString(__FUNCTION__), *OriginHandleJson);
		}

		FXGSampleRespSteamMiddleMessage StreamMiddleMessage;

		FJsonObjectConverter::JsonObjectStringToUStruct(HandleJson, &StreamMiddleMessage);


		FXGSampleBDRespInfo RespInfo;
		RespInfo.bResult = true;
		RespInfo.bReqStream = true;
		RespInfo.Message = StreamMiddleMessage.result;
		RespInfo.RawMessage = OriginHandleJson;
		RespInfo.SteamMiddleMessage = StreamMiddleMessage;


		TotalString += RespInfo.Message;

		CallOnUpdate(AsyncID, true, RespInfo.Message, RespInfo);
	
		RecursiveProcessingStreamData();

	}

}

void UXGSampleBDAyncAction::CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleBDRespInfo RespInfo)
{
	FXGSampleBDLinkDelegate TempDelegate = OnSuccess;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});

}

void UXGSampleBDAyncAction::CallOnUpdate(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleBDRespInfo RespInfo)
{
	FXGSampleBDLinkDelegate TempDelegate = OnUpdate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGSampleBDAyncAction::CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGSampleBDRespInfo RespInfo)
{
	FXGSampleBDLinkDelegate TempDelegate = OnFail;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGSampleBDAyncAction::RealeaseResources()
{

	SetReadyToDestroy();
}




int TestFunc()
 {
	return 123;
	
}
int TestFunc1(FString Name)
{
	return 123;
}

int TestFunc2()
{
	TFunction<int()> Task = TestFunc;

	TFunction<int(FString)> Task1 = TestFunc1;

	TFunction<int()> Task2 = []()->int{
		TestFunc1(TEXT("这样可以"));
		return 11;
	};

	auto Result = Async(EAsyncExecution::Thread, Task);

	//auto Result1 = Async(EAsyncExecution::Thread, Task1(TEXT("11")));

	auto Resul2 = Async(EAsyncExecution::Thread, Task2);


	return 11;
}
