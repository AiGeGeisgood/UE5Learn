#include "XGSampleSTTSubsystem.h"

#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "HAL/RunnableThread.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Base64.h"



#include "LogXGXF.h"
#include "XGSampleAudioCaptureSubsystem.h"
#include "XGSampleConsumeVoiceRunnable.h"


UXGSampleSTTSubsystem* UXGSampleSTTSubsystem::RealTimeSTTSubsystemPtr = nullptr;

bool UXGSampleSTTSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;


}

void UXGSampleSTTSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RealTimeSTTSubsystemPtr = this;
}

void UXGSampleSTTSubsystem::Deinitialize()
{
	XGForceToStopRealTimeSPeechToText(true);

	RealTimeSTTSubsystemPtr = nullptr;

	Super::Deinitialize();

}

bool UXGSampleSTTSubsystem::IsRealTimeSTTReady()
{

	return STTStatus == EXGSampleSTTStatus::Ready;
}

FGuid UXGSampleSTTSubsystem::XGBeginRealTimeSpeechToText(FString InAppID,
	FString InAPIKey,
	FXGSampleSTTReqInfo InSTTReqInfo,
	FXGSampleInitSTTDelegate InSTTInitDelegate,
	FXGSampleSTTRespDelegate InSTTRespDelegate,
	FXGSampleSTTCloseDelegate InSTTCloseDelegate)
{
	AsyncID = FGuid::NewGuid();

	STTInitDelegate = InSTTInitDelegate;

	if (!IsRealTimeSTTReady())
	{
		CallInitRealTimeSTTDelegate(AsyncID, false,
			TEXT("XGSampleSTTSubsystem is Running ,please close it!"),
			FXGSampleSTTInitInformation());

		UE_LOG(LogXGXFSTT, Warning, TEXT("[%s]-AsyncID:[%s] try to connect to iFLyTek,But It is Running"), *FString(__FUNCTION__), *(AsyncID.ToString()));
	}


	UE_LOG(LogXGXFSTT, Display, TEXT("[%s]-AsyncID:[%s] is trying to connect to iFLyTek"), *FString(__FUNCTION__), *(AsyncID.ToString()));



	STTStatus = EXGSampleSTTStatus::Init;

	AppID = InAppID;

	APIKey = InAPIKey;

	STTRespDelegate = InSTTRespDelegate;

	STTCloseDelegate = InSTTCloseDelegate;

	FModuleManager::Get().LoadModuleChecked("WebSockets");

	FString ServerProtocol = TEXT("ws");
	FString ServerURL = TEXT("ws://rtasr.xfyun.cn/v1/ws?");

	ServerURL += InSTTReqInfo.GenerateRequireParams(AppID, APIKey);

	AsyncTask(ENamedThreads::GameThread, [ServerURL, ServerProtocol, this]() {

		FModuleManager::Get().LoadModuleChecked("WebSockets");

		Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

		Socket->OnConnected().AddUObject(this, &UXGSampleSTTSubsystem::OnConnected);
		Socket->OnConnectionError().AddUObject(this, &UXGSampleSTTSubsystem::OnConnectionError);
		Socket->OnClosed().AddUObject(this, &UXGSampleSTTSubsystem::OnClosed);
		Socket->OnMessage().AddUObject(this, &UXGSampleSTTSubsystem::OnMessage);
		Socket->OnMessageSent().AddUObject(this, &UXGSampleSTTSubsystem::OnMessageSent);

		Socket->Connect();

		});






	return AsyncID;
}

void UXGSampleSTTSubsystem::XGStopRealTimeSpeechToText()
{
	if (STTStatus == EXGSampleSTTStatus::Ready)
	{
		UE_LOG(LogXGXFSTT, Warning,
		TEXT("[%s]-AsyncID:[%s] try to close iFLyTek connect,but It was closed"), *FString(__FUNCTION__), *(AsyncID.ToString()));
		return;
	}

	STTStatus = EXGSampleSTTStatus::WaitToServerClose;

	UE_LOG(LogXGXFSTT, Warning, TEXT("[%s]-AsyncID:[%s] try to close iFLyTek Connect"), *FString(__FUNCTION__), *(AsyncID.ToString()));


	RealseaseVoiceGenerateRunnale();

	RealeaseVoiceConsumeRunnable();

	EndSendVoiceData();

}

void UXGSampleSTTSubsystem::XGForceToStopRealTimeSPeechToText(bool IsDeinitialize)
{
	if (STTStatus == EXGSampleSTTStatus::Ready)
	{

		if (IsDeinitialize)
		{

		}
		else
		{

			UE_LOG(LogXGXFSTT, Warning,
			TEXT("[%s]-AsyncID:[%s] try to force to close iFLyTek connect,but It was closed"), *FString(__FUNCTION__), *(AsyncID.ToString()));

		}


	}
	else
	{

		UE_LOG(LogXGXFSTT, Warning, TEXT("[%s]-AsyncID:[%s]  force to close iFLyTek connect."), *FString(__FUNCTION__), *(AsyncID.ToString()));


		RealseaseVoiceGenerateRunnale();

		RealeaseVoiceConsumeRunnable();

		EndSendVoiceData();

		RealeaseWebSocket();

		RealeaseSTTInfo();

		STTStatus = EXGSampleSTTStatus::Ready;

	}


}

bool UXGSampleSTTSubsystem::XGIsRealTimeSpeechToTextWorking()
{

	return STTStatus != EXGSampleSTTStatus::Ready;
}

void UXGSampleSTTSubsystem::CallInitRealTimeSTTDelegate(FGuid InSTTAsyncID,
	bool bInInitResult,
	FString InInitRespMessage,
	FXGSampleSTTInitInformation InSTTInitInformation)
{
	FXGSampleInitSTTDelegate TempDelegate = STTInitDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InSTTAsyncID, bInInitResult, InInitRespMessage, InSTTInitInformation);

		});

}

void UXGSampleSTTSubsystem::CallRealTimeSTTRespDelegate(FXGSampleSTTInformation InSTTInformation)
{

	FXGSampleSTTRespDelegate TempDelegate = STTRespDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InSTTInformation);

		});

}

void UXGSampleSTTSubsystem::CallRealTimeSTTCloseDelegate(FGuid InSTTAsyncID, int32 InStatusCode, FString InReason, bool bInWasClean)
{
	FXGSampleSTTCloseDelegate TempDelegate = STTCloseDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InSTTAsyncID, InStatusCode, InReason, bInWasClean);

		});
}

void UXGSampleSTTSubsystem::RealseaseVoiceGenerateRunnale()
{

	UXGSampleAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGSampleAudioCaptureSubsystem>();

	if (XunFeiAudioCaptureSubsystem)
	{
		XunFeiAudioCaptureSubsystem->StopCapturingAudio();
	}


}

void UXGSampleSTTSubsystem::RealeaseVoiceConsumeRunnable()
{
	if (ConsumeVoiceRunnable.IsValid())
	{
		ConsumeVoiceRunnable->Stop();
	}

	if (RunnableThread)
	{
		RunnableThread->WaitForCompletion();
		delete RunnableThread;
		RunnableThread = nullptr;
	}



}

void UXGSampleSTTSubsystem::RealeaseWebSocket()
{

	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}

	Socket.Reset();

}

void UXGSampleSTTSubsystem::RealeaseSTTInfo()
{


	AppID = TEXT("");

	APIKey = TEXT("");

	AsyncID = FGuid();

	STTInitDelegate.Clear();

	STTRespDelegate.Clear();

	STTCloseDelegate.Clear();
}

void UXGSampleSTTSubsystem::SendVoiceData(const TArray<float>& InVoiceData)
{
	TArray<int16> ToChangeAuidoData;

	TArray<uint8> BinaryDataToSend;

	for (int32 VoiceIndex = 0; VoiceIndex < InVoiceData.Num(); VoiceIndex++)
	{
		ToChangeAuidoData.Add((int16)FMath::Clamp<int32>(FMath::FloorToInt(32767.0f * InVoiceData[VoiceIndex]), -32768, 32767));

		uint8 Bytes[2];

		Bytes[0] = (uint8)(ToChangeAuidoData[VoiceIndex] & 0xFF);

		Bytes[1] = (uint8)((ToChangeAuidoData[VoiceIndex] >> 8) & 0xFF);

		BinaryDataToSend.Add(Bytes[0]);

		BinaryDataToSend.Add(Bytes[1]);

	}
	AsyncTask(ENamedThreads::GameThread, [BinaryDataToSend]() {

		if (UXGSampleSTTSubsystem::RealTimeSTTSubsystemPtr && UXGSampleSTTSubsystem::RealTimeSTTSubsystemPtr->STTStatus == EXGSampleSTTStatus::Processing)
		{
			if (RealTimeSTTSubsystemPtr->Socket.IsValid() && RealTimeSTTSubsystemPtr->Socket->IsConnected())
			{
				RealTimeSTTSubsystemPtr->Socket->Send(BinaryDataToSend.GetData(), BinaryDataToSend.Num(), true);

			}
		}

		});



}

void UXGSampleSTTSubsystem::EndSendVoiceData()
{

	//这个写法有点危险
	if (Socket.IsValid() && Socket->IsConnected())
	{
		FString EndStr = TEXT("{\"end\": true}");

		int32 Length = strlen(TCHAR_TO_UTF8(*EndStr));

		Socket->Send(TCHAR_TO_UTF8(*EndStr), Length, true);
	}


}

void UXGSampleSTTSubsystem::OnConnected()
{
	UE_LOG(LogXGXFSTT, Display, TEXT("[%s]-AsyncID:[%s] Connect Succeess!"), *FString(__FUNCTION__), *(AsyncID.ToString()));
}

void UXGSampleSTTSubsystem::OnConnectionError(const FString& ErrorMessage)
{

	UE_LOG(LogXGXFSTT, Error, TEXT("[%s]-AsyncID:[%s] ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *(AsyncID.ToString()), *ErrorMessage);

	RealeaseWebSocket();

	STTStatus = EXGSampleSTTStatus::Ready;

	CallInitRealTimeSTTDelegate(AsyncID, false, ErrorMessage, FXGSampleSTTInitInformation());

	RealeaseSTTInfo();

}

void UXGSampleSTTSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{

	UE_LOG(LogXGXFSTT, Warning, TEXT("[%s]-AsyncID:[%s] ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), *(AsyncID.ToString()), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));


	RealseaseVoiceGenerateRunnale();

	RealeaseVoiceConsumeRunnable();

	RealeaseWebSocket();

	STTStatus = EXGSampleSTTStatus::Ready;

	CallRealTimeSTTCloseDelegate(AsyncID, StatusCode, Reason, bWasClean);

	RealeaseSTTInfo();


}

void UXGSampleSTTSubsystem::OnMessage(const FString& Message)
{
	UE_LOG(LogXGXFSTT, Verbose, TEXT("[%s]-AsyncID:[%s], Message:%s"), *(AsyncID.ToString()), *FString(__FUNCTION__), *Message);

	FXGSampleSTTRespInfo RespInfo;

	if (!Message.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Message, &RespInfo, 0, 0))
	{

		if (RespInfo.action.Equals(TEXT("started")))
		{

			if (STTStatus != EXGSampleSTTStatus::Init)
			{

				UE_LOG(LogXGXFSTT, Error, 
				TEXT("[%s]-AsyncID:[%s] Unknow Procedure Error"),
				*FString(__FUNCTION__), *(AsyncID.ToString()));

				FXGSampleSTTInitInformation InitInformation;
				InitInformation.RespInfo = RespInfo;

				CallInitRealTimeSTTDelegate(AsyncID, false, TEXT("Unknow Procedure Error"), InitInformation);
				XGForceToStopRealTimeSPeechToText();
				return;
			}

			STTStatus = EXGSampleSTTStatus::Processing;


			UXGSampleAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGSampleAudioCaptureSubsystem>();

			FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

			XunFeiAudioCaptureSubsystem->StartCapturingAudio();

			bool bGetAudioDevice = XunFeiAudioCaptureSubsystem->GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);

			bool bRightAudioRateAndChannles = AudioCaptureDeviceInfo.SampleRate == 48000 && AudioCaptureDeviceInfo.NumInputChannels == 2;

			bRightAudioRateAndChannles = true;

			if (!bGetAudioDevice || !bRightAudioRateAndChannles)
			{
				FString AudioErrorMessage = TEXT("AudioCaptureDevice is not right!");

				UE_LOG(LogXGXFSTT, Error, TEXT("[%s]-AsyncID:[%s]--[%s]"), *FString(__FUNCTION__),
				*(AsyncID.ToString()), *AudioErrorMessage);

				XunFeiAudioCaptureSubsystem->StopCapturingAudio();

				FXGSampleSTTInitInformation InitInformation;
				InitInformation.RespInfo = RespInfo;

				CallInitRealTimeSTTDelegate(AsyncID, false, AudioErrorMessage, InitInformation);

				XGForceToStopRealTimeSPeechToText();

				return;
			}

			UE_LOG(LogXGXFSTT, Warning, TEXT("[%s]-AsyncID:[%s]:Audio DeviceName:[%s],NumInputChannels:[%d],SampleRate:[%d]"),
				*FString(__FUNCTION__),
				*(AsyncID.ToString()),
				*(AudioCaptureDeviceInfo.DeviceName.ToString()),
				AudioCaptureDeviceInfo.NumInputChannels,
				AudioCaptureDeviceInfo.SampleRate);


			ConsumeVoiceRunnable = MakeShared<FXGSampleConsumeVoiceRunnable>(TEXT("XunFeiConsumeVoiceRunnable"));

			RunnableThread = FRunnableThread::Create(ConsumeVoiceRunnable.Get(), *(ConsumeVoiceRunnable->ThreadName));

			FString InitMessage = TEXT("XunFeiRealTimeSTT Init Success !");

			FXGSampleSTTInitInformation InitInformation;
			InitInformation.RespInfo = RespInfo;


			CallInitRealTimeSTTDelegate(AsyncID, true, InitMessage, InitInformation);

			UE_LOG(LogXGXFSTT, Display, TEXT("[%s]-AsyncID:[%s]--[%s]"), *FString(__FUNCTION__), *(AsyncID.ToString()), *InitMessage);



		}
		else if (RespInfo.action.Equals(TEXT("error")))
		{
			UE_LOG(LogXGXFSTT, Error,
			TEXT("[%s]-AsyncID:[%s]--Error--[%s]"), *FString(__FUNCTION__), *(AsyncID.ToString()), *(RespInfo.desc));

			FXGSampleSTTInitInformation InitInformation;

			InitInformation.RespInfo = RespInfo;

			CallInitRealTimeSTTDelegate(AsyncID, false, *(RespInfo.desc), InitInformation);

			XGStopRealTimeSpeechToText();

			return;


		}
		else if (RespInfo.action.Equals(TEXT("result")))
		{
			TSharedPtr<FJsonObject> ResultDataObj;
			TSharedRef<TJsonReader<>> DataReader = TJsonReaderFactory<>::Create(RespInfo.data);
			FJsonSerializer::Deserialize(DataReader, ResultDataObj);

			bool bFindDiz = ResultDataObj->HasField(TEXT("biz"));
			if (bFindDiz)
			{

				FXGSampleSTTTranslateData RealTimeSTTTranslateData;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(RespInfo.data, &RealTimeSTTTranslateData, 0, 0))
				{
					if (RealTimeSTTTranslateData.ed > 0)
					{
						FString Dst = RealTimeSTTTranslateData.dst;
						FString Src = RealTimeSTTTranslateData.src;

						UE_LOG(LogXGXFSTT, Log, TEXT("[%s]-AsyncID [%s]: [dst]-[%s]||[src]-[%s]"), *FString(__FUNCTION__), *(AsyncID.ToString()), *Dst, *Src);


						FXGSampleSTTInformation RealTimeSTTInformation;

						RealTimeSTTInformation.AsyncSTTID = AsyncID;
						RealTimeSTTInformation.bSuccess = true;
						RealTimeSTTInformation.RespType = EXGSampleSTTRespType::FinalTextWithSrcAndDst;
						RealTimeSTTInformation.SrcText = Src;
						RealTimeSTTInformation.DstText = Dst;

						RealTimeSTTInformation.RawInformation.RealTImeSTTRespInfo = RespInfo;
						RealTimeSTTInformation.RawInformation.RealTimeSTTTranslateData = RealTimeSTTTranslateData;

						CallRealTimeSTTRespDelegate(RealTimeSTTInformation);


					}


				}

			}
			else
			{

				FXGSampleSTTNoTranslateData RealTimeSTTNoTranslateData;

				if (FJsonObjectConverter::JsonObjectStringToUStruct(RespInfo.data, &RealTimeSTTNoTranslateData, 0, 0))
				{

					bool bMiddle = RealTimeSTTNoTranslateData.cn.st.type.Equals(TEXT("1"));

					if (bMiddle)
					{

						TArray<FXGSampleSTTNoTranslateWSData> WSDatas = RealTimeSTTNoTranslateData.cn.st.rt[0].ws;

						FString OutText = TEXT("");
						FString OutRoleType = TEXT("0");

						for (auto& TmpWs : WSDatas)
						{
							for (auto& TmpCW : TmpWs.cw)
							{
								OutText += TmpCW.w;

								if (!TmpCW.rl.Equals(TEXT("0")))
								{
									OutRoleType = TmpCW.rl;
								}


							}
						}

						FXGSampleSTTInformation RealTimeSTTInformation;

						RealTimeSTTInformation.AsyncSTTID = AsyncID;
						RealTimeSTTInformation.bSuccess = true;
						RealTimeSTTInformation.RespType = EXGSampleSTTRespType::MiddleTextOnlySrc;
						RealTimeSTTInformation.SrcText = OutText;
						RealTimeSTTInformation.RoleType = OutRoleType;

						RealTimeSTTInformation.RawInformation.RealTImeSTTRespInfo = RespInfo;
						RealTimeSTTInformation.RawInformation.RealTimeSTTNoTranslateData = RealTimeSTTNoTranslateData;

						CallRealTimeSTTRespDelegate(RealTimeSTTInformation);

					}
					else
					{

						TArray<FXGSampleSTTNoTranslateWSData> WSDatas = RealTimeSTTNoTranslateData.cn.st.rt[0].ws;

						FString OutText;
						FString OutRoleType = TEXT("0");

						for (auto& TmpWs : WSDatas)
						{
							for (auto& TmpCW : TmpWs.cw)
							{
								OutText += TmpCW.w;

								if (!TmpCW.rl.Equals(TEXT("0")))
								{
									OutRoleType = TmpCW.rl;
								}

							}
						}

						UE_LOG(LogXGXFSTT, Verbose, TEXT("[%s]-AsyncID-[%s]:[src]-[%s] "), *FString(__FUNCTION__), *(AsyncID.ToString()), *OutText);

						FXGSampleSTTInformation RealTimeSTTInformation;

						RealTimeSTTInformation.AsyncSTTID = AsyncID;
						RealTimeSTTInformation.bSuccess = true;
						RealTimeSTTInformation.RespType = EXGSampleSTTRespType::FinalTextOnlySrc;
						RealTimeSTTInformation.SrcText = OutText;
						RealTimeSTTInformation.RoleType = OutRoleType;

						RealTimeSTTInformation.RawInformation.RealTImeSTTRespInfo = RespInfo;
						RealTimeSTTInformation.RawInformation.RealTimeSTTNoTranslateData = RealTimeSTTNoTranslateData;

						CallRealTimeSTTRespDelegate(RealTimeSTTInformation);

					}


				}
			}


		}


	}
	else
	{
		//check(false);
	}



}

void UXGSampleSTTSubsystem::OnMessageSent(const FString& MessageString)
{
	UE_LOG(LogXGXFSTT, Display, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *MessageString);

}
