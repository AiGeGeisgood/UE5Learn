#include "XGSampleTTSAsyncAction.h"

#include "WebSocketsModule.h"
#include "IWebSocket.h"

#include "JsonObjectConverter.h"

#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "Misc/FileHelper.h"

#include "Containers/StringConv.h"
#include "Sound/SoundWave.h"
#include "Runtime/Launch/Resources/Version.h"
#include "UObject/Package.h"

#include "Sound/SoundWaveProcedural.h"

#include "SampleBuffer.h" 
#include "Sound/SampleBufferIO.h"


#include "LogXGXF.h"
#include "XGSampleXFLinkBase.h"


UXGSampleTTSAsyncAction::UXGSampleTTSAsyncAction(const FObjectInitializer& ObjectInitializer)
{
}

UXGSampleTTSAsyncAction::~UXGSampleTTSAsyncAction()
{
	ForceToCloseXunFeiTTsSocket();
}

UXGSampleTTSAsyncAction* UXGSampleTTSAsyncAction::XGXunFeiTextToSpeech(UObject* WorldContextObject, 
FString InTTSAppID, 
FString InTTSAPISecret, 
FString InTTSAPIKey, 
const FString& InText,
bool bInSaveToLocal, 
const FString& InSaveFileFullPath, 
FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo)
{
	check(WorldContextObject);

	UXGSampleTTSAsyncAction* AsyncAction = NewObject<UXGSampleTTSAsyncAction>();

	AsyncAction->AppID = InTTSAppID;
	AsyncAction->APISecret = InTTSAPISecret;
	AsyncAction->APIKey = InTTSAPIKey;

	AsyncAction->SetXunFeiText(InText);
	AsyncAction->SetIsSaveToLocal(bInSaveToLocal);
	AsyncAction->SetSaveFileFullPath(InSaveFileFullPath);
	AsyncAction->SetXunFeiTTSReqInfo(InXunFeiTTSReqInfo);

	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;

}

void UXGSampleTTSAsyncAction::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});

	Then.Broadcast(AsyncID, false, TEXT("TTS is just started.Please wait to finish ! "), nullptr);
}

void UXGSampleTTSAsyncAction::Activate_Internal()
{
	if (bSaveToLocal)
	{
		FString FileDirectory = FPaths::GetPath(SaveFileFullPath);
		bool bDirectory = FPaths::DirectoryExists(FileDirectory);
		FString FileExtension = FPaths::GetExtension(SaveFileFullPath);
		bool bExtension = FileExtension.Equals(TEXT("wav"));

		if (!bDirectory)
		{
			FString DirectoryErrorMessage = FString::Printf(TEXT("FileDirectory don't exist ! FilePath: [%s]"), *SaveFileFullPath);

			UE_LOG(LogXGXFSTT, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *DirectoryErrorMessage);

			CallOnSoundWaveFail(false, DirectoryErrorMessage);

			RealeaseResources();

			return;
		}


		if (!bExtension)
		{
			FString FileNameErroeMessage = FString::Printf(TEXT("FileName is Wrong,it must endwith [.wav] ! FilePath: [%s]"), *FString(__FUNCTION__), *SaveFileFullPath);

			UE_LOG(LogXGXFSTT, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *FileNameErroeMessage);

			CallOnSoundWaveFail(false, FileNameErroeMessage);

			RealeaseResources();

			return;
		}

	}



	if (XunFeiText.IsEmpty())
	{

		FString TextErrorMessage = FString::Printf(TEXT("InText is Empty,it must be input!"));

		UE_LOG(LogXGXFSTT, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *TextErrorMessage);

		CallOnSoundWaveFail(false, TextErrorMessage);

		RealeaseResources();

		return;
	}


	AsyncTask(ENamedThreads::GameThread, [this]() {

		CreateXunFeiTTSSocket();

		});








}

bool UXGSampleTTSAsyncAction::LoadLocalFileData(FString InPath, TArray<uint8>& OutBinaryData)
{
	
	if (FPaths::FileExists(InPath))
	{
		OutBinaryData.Empty();
		return  FFileHelper::LoadFileToArray(OutBinaryData, *InPath);
	}

	return false;
}

void UXGSampleTTSAsyncAction::RealeaseResources()
{
	AppID = TEXT("");

	APISecret = TEXT("");

	APIKey = TEXT("");

	XunFeiText.Empty();

	bSaveToLocal = false;

	SaveFileFullPath = TEXT("");

	XunFeiTTSReqInfo = FXGXunFeiTTSReqInfo();



	OnSoundWaveSuccess.Clear();

	OnSoundWaveFail.Clear();

	OnWavFileSuccess.Clear();

	OnWavFileFail.Clear();

	Socket.Reset();

	XunFeiAudioData.Empty();

	SetReadyToDestroy();
}

void UXGSampleTTSAsyncAction::CreateXunFeiTTSSocket()
{

	FString iFlyTekURL = TEXT("");



	iFlyTekURL = TEXT("ws://tts-api.xfyun.cn/v2/tts");



	FString ServerURL = TEXT("");

	FString ServerProtocol = TEXT("");

	FXGSampleXFLinkBase::AssembleAuthUrl(iFlyTekURL,APISecret,APIKey, ServerURL, ServerProtocol,true);

	FModuleManager::Get().LoadModuleChecked("WebSockets");

	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);


	Socket->OnConnected().AddUObject(this, &UXGSampleTTSAsyncAction::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGSampleTTSAsyncAction::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGSampleTTSAsyncAction::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGSampleTTSAsyncAction::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGSampleTTSAsyncAction::OnMessageSent);

	Socket->Connect();


}

void UXGSampleTTSAsyncAction::CloseXunFeiTTsSocket()
{
	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}


}

void UXGSampleTTSAsyncAction::ForceToCloseXunFeiTTsSocket()
{
	CloseXunFeiTTsSocket();

	RealeaseResources();

}

void UXGSampleTTSAsyncAction::OnConnected()
{
	UE_LOG(LogXGXFSTT, Display, TEXT("[%s]:[%s] Connect Success!"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString Base64XunFeiText = TCharStringToUTF8Base64String(XunFeiText);

	XunFeiTTSReqInfo.common.app_id = AppID;

	XunFeiTTSReqInfo.data.text = Base64XunFeiText;

	XunFeiTTSReqInfo.business.speed = FMath::Clamp(XunFeiTTSReqInfo.business.speed, 0, 100);
	XunFeiTTSReqInfo.business.volume = FMath::Clamp(XunFeiTTSReqInfo.business.volume, 0, 100);
	XunFeiTTSReqInfo.business.pitch = FMath::Clamp(XunFeiTTSReqInfo.business.pitch, 0, 100);

	FString JsonStr;
	FJsonObjectConverter::UStructToJsonObjectString(XunFeiTTSReqInfo, JsonStr);


	Socket->Send(JsonStr);
}

void UXGSampleTTSAsyncAction::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGXFSTT, Error, TEXT("[%s]:[%s] ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *(AsyncID.ToString()), *ErrorMessage);

	CallOnSoundWaveFail(false, ErrorMessage);

	RealeaseResources();
}

void UXGSampleTTSAsyncAction::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGXFSTT, Warning, TEXT("[%s]:[%s] ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), *(AsyncID.ToString()), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));

	RealeaseResources();

}

void UXGSampleTTSAsyncAction::OnMessage(const FString& Message)
{

	//UE_LOG(LogXGXFSTT, Log, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *Message);

	FXGXunFeiTTSRespInfo XunFeiTTSRespInfo;
	FJsonObjectConverter::JsonObjectStringToUStruct(Message, &XunFeiTTSRespInfo, 0, 0);

	if (XunFeiTTSRespInfo.code == 0)
	{

		FString Audio = XunFeiTTSRespInfo.data.audio;
		int32	Status = XunFeiTTSRespInfo.data.status;
		FString Ced = XunFeiTTSRespInfo.data.ced;


		bool bRightStatus = Status == 1 || Status == 2;

		if (!Audio.IsEmpty() && bRightStatus)
		{

			TArray<uint8> AudioData;
			FBase64::Decode(Audio, AudioData);
			XunFeiAudioData.Append(AudioData);
		}

		if (Status == 2)
		{

			if (bSaveToLocal)
			{

				CallGenerateWavFile();

			}

			USoundWave* SoundWave = ImportPCMToSoundWave(XunFeiAudioData);

		//	USoundWave* SoundWave = ImportPCMToSoundWave_Method1(XunFeiAudioData);
		//	USoundWave* SoundWave = ImportPCMToSoundWave_Method2(XunFeiAudioData);
			
			UE_LOG(LogXGXFSTT, Log, TEXT("[%s]:Text To SPeech Success!"), *FString(__FUNCTION__));

			CallOnSoundWaveSuccess(true, TEXT("Text To SPeech Success!"), SoundWave);


			CloseXunFeiTTsSocket();

		}

	}
	else
	{

		FString ErrorMessage = FString::Printf(TEXT("MessageError-Code:[%d]-Message:[%s]-sid-[%s]"),
			XunFeiTTSRespInfo.code,
			*(XunFeiTTSRespInfo.message),
			*(XunFeiTTSRespInfo.sid));

		UE_LOG(LogXGXFSTT, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *ErrorMessage);


		CallOnSoundWaveFail(false, ErrorMessage);

		CloseXunFeiTTsSocket();
	}





}

void UXGSampleTTSAsyncAction::OnMessageSent(const FString& MessageString)
{
	UE_LOG(LogXGXFSTT, Log, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *MessageString);

}

void UXGSampleTTSAsyncAction::CallOnSoundWaveSuccess(bool bInResult, FString InMessage, USoundWave* InSoundWavePtr)
{
	FXGXunFeiTTSDelegate TempXunFeiTTSDelegate = OnSoundWaveSuccess;
	FGuid InAsyncID = AsyncID;

	InSoundWavePtr->AddToRoot();

	AsyncTask(ENamedThreads::GameThread, [=]() {

		InSoundWavePtr->RemoveFromRoot();

		TempXunFeiTTSDelegate.Broadcast(InAsyncID, bInResult, InMessage, InSoundWavePtr);


		});

}

void UXGSampleTTSAsyncAction::CallOnSoundWaveFail(bool bInResult, FString InMessage, USoundWave* InSoundWavePtr)
{
	FXGXunFeiTTSDelegate TempXunFeiTTSDelegate = OnSoundWaveFail;
	FGuid InAsyncID = AsyncID;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempXunFeiTTSDelegate.Broadcast(InAsyncID, bInResult, InMessage, InSoundWavePtr);


		});
}

void UXGSampleTTSAsyncAction::CallGenerateWavFile()
{

	FXGXunFeiTTSDelegate TempOnWavFileSuccess = OnWavFileSuccess;
	FXGXunFeiTTSDelegate TempOnWavFileFail = OnWavFileFail;


	TArray<uint8> AudioData = XunFeiAudioData;
	FString WavFilePath = SaveFileFullPath;
	FGuid OutAsyncID = AsyncID;


	AsyncTask(ENamedThreads::AnyThread, [TempOnWavFileSuccess, TempOnWavFileFail, AudioData, WavFilePath, OutAsyncID]() {

		TArray<uint8> WaveData;
		UXGSampleTTSAsyncAction::ConvertPCMToWave(AudioData, WaveData);

		bool bSave = FFileHelper::SaveArrayToFile(WaveData, *WavFilePath);

		FXGXunFeiTTSDelegate TempXunFeiTTSDelegate;
		FString WavFileMessage = TEXT("");

		if (bSave)
		{
			TempXunFeiTTSDelegate = TempOnWavFileSuccess;
			WavFileMessage = TEXT("The wav file has been finished !");
		}
		else
		{
			TempXunFeiTTSDelegate = TempOnWavFileFail;
			WavFileMessage = TEXT("The wav file failed to be generated");
		}

		AsyncTask(ENamedThreads::GameThread, [TempXunFeiTTSDelegate, bSave, WavFileMessage, OutAsyncID]() {

			TempXunFeiTTSDelegate.Broadcast(OutAsyncID, bSave, WavFileMessage, nullptr);

			});






	});




}

FString UXGSampleTTSAsyncAction::TCharStringToUTF8Base64String(const FString& InTCharString)
{

	FString Base64XunFeiText = TEXT("");

#if ENGINE_MAJOR_VERSION == 4 || ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION==0

	FTCHARToUTF8 UTF8String(*InTCharString);

	Base64XunFeiText = FBase64::Encode((uint8*)UTF8String.Get(), UTF8String.Length() * sizeof(uint8));

#endif

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >=1

	auto UTF8String = StringCast<UTF8CHAR>(*InTCharString);

	Base64XunFeiText = FBase64::Encode((uint8*)UTF8String.Get(), UTF8String.Length());

#endif

	return Base64XunFeiText;
}

FString UXGSampleTTSAsyncAction::Base64UTF8StringToTCharString(const FString& InBase64UTF8String)
{
	FString TextString = TEXT("");

	TArray<uint8> TextData;

	FBase64::Decode(InBase64UTF8String, TextData);

#if ENGINE_MAJOR_VERSION == 4 || ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION==0

	FUTF8ToTCHAR TCHARData(reinterpret_cast<const ANSICHAR*>(TextData.GetData()), TextData.Num());

	TextString = FString(TCHARData.Length(), TCHARData.Get());

#endif 


#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >=1

	TextString = FUTF8ToTCHAR(reinterpret_cast<const UTF8CHAR*>(TextData.GetData()), TextData.Num());

#endif 

	return TextString;

}

USoundWave* UXGSampleTTSAsyncAction::ImportPCMToSoundWave(const TArray<uint8>& InPCMData)
{
	//目前这个方法在5.4.3,5.4.4无法正常使用

	const int32 ChannelCount = 1;

	const int32 SizeOfSample = 16 / 8;

	const int32 NumSamples = InPCMData.Num() / SizeOfSample;

	const int32 NumFrames = NumSamples / ChannelCount;

	const int32 SampleDataSize = InPCMData.Num();

	FString SoundWaveName = TEXT("XGSoundWave:") + FGuid::NewGuid().ToString();

	USoundWave* SoundWave = NewObject < USoundWave>(GetTransientPackage(), *SoundWaveName, RF_Public | RF_Standalone);




#if WITH_EDITOR&& ENGINE_MAJOR_VERSION == 5&& ENGINE_MINOR_VERSION >=1 

	TArray<uint8> WavData;

	ConvertPCMToWave(InPCMData, WavData);

	SoundWave->RawData.UpdatePayload(FSharedBuffer::Clone(WavData.GetData(), WavData.Num()));

#endif  


	SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SampleDataSize);

	FMemory::Memcpy(SoundWave->RawPCMData, InPCMData.GetData(), SampleDataSize);

	SoundWave->RawPCMDataSize = SampleDataSize;

	// Set Sound Wave Info
	SoundWave->Duration = (float)NumFrames / 16000.f;
	SoundWave->SetSampleRate(16000.f);
	SoundWave->NumChannels = ChannelCount;
	SoundWave->TotalSamples = NumFrames;



	return SoundWave;
}

USoundWave* UXGSampleTTSAsyncAction::ImportPCMToSoundWave_Method1(const TArray<uint8>& InPCMData)
{
	const int32 ChannelCount = 1;

	const int32 SizeOfSample = 16 / 8;

	const int32 NumSamples = InPCMData.Num() / SizeOfSample;

	const int32 NumFrames = NumSamples / ChannelCount;

	const int32 SampleDataSize = InPCMData.Num();

	FString SoundWaveName = TEXT("SoundWave:") + FGuid::NewGuid().ToString();
	USoundWave* SoundWave = NewObject<USoundWave>(GetTransientPackage(), *SoundWaveName, RF_Public | RF_Standalone);


	TArray<uint8> WavData;

	ConvertPCMToWave(InPCMData, WavData);

#if WITH_EDITOR&& ENGINE_MAJOR_VERSION == 5&& ENGINE_MINOR_VERSION >=1 

	FSharedBuffer Buffer = FSharedBuffer::Clone(WavData.GetData(), WavData.Num());

	SoundWave->RawData.UpdatePayload(Buffer);

#endif 

	FWaveModInfo WaveInfo;

	WaveInfo.ReadWaveInfo(WavData.GetData(), WavData.Num());

	SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);

	SoundWave->NumChannels = *WaveInfo.pChannels;

	const int32 BytesDataPerSecond = *WaveInfo.pChannels * (*WaveInfo.pBitsPerSample / 8.f) * *WaveInfo.pSamplesPerSec;


	SoundWave->Duration = WaveInfo.SampleDataSize / BytesDataPerSecond;

	SoundWave->RawPCMDataSize = WaveInfo.SampleDataSize;

	SoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;

	SoundWave->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));

	FMemory::Memcpy(SoundWave->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);

	SoundWave->SetSoundAssetCompressionType(ESoundAssetCompressionType::PCM);

	// SoundWave-> SoundWaveDataPtr->InitializeDataFromSoundWave(*SoundWave);

	return SoundWave;
}

USoundWave* UXGSampleTTSAsyncAction::ImportPCMToSoundWave_Method2(const TArray<uint8>& InPCMData)
{
	const int32 ChannelCount = 1;

	const int32 SizeOfSample = 16 / 8;

	const int32 NumSamples = InPCMData.Num() / SizeOfSample;

	const int32 NumFrames = NumSamples / ChannelCount;

	const int32 SampleDataSize = InPCMData.Num();

	TArray<int16> ToChangeAuidoData;

	ToChangeAuidoData.SetNumUninitialized(NumSamples);

	FMemory::Memcpy(ToChangeAuidoData.GetData(), InPCMData.GetData(), InPCMData.Num());

	Audio::FSampleBuffer SampleBuffer(ToChangeAuidoData.GetData(), NumSamples, 1, 16000);
	Audio::FSoundWavePCMWriter Writer;

	USoundWave* SoundWave2 = Writer.SynchronouslyWriteSoundWave(SampleBuffer);

	//SoundWave2->SetSoundAssetCompressionType(ESoundAssetCompressionType::PCM);
	//SoundWave2->SoundWaveDataPtr->InitializeDataFromSoundWave(*SoundWave2);

	return SoundWave2;
}

void UXGSampleTTSAsyncAction::ConvertPCMToWave(const TArray<uint8>& InPCMData, TArray<uint8>& OutWaveData)
{
	OutWaveData.Empty();

	FXGWaveHeard WaveHeard;
	FXGWaveFmt WaveFmt;
	FXGWaveData WaveData;

	FMemory::Memcpy(WaveHeard.ChunkID, "RIFF", strlen("RIFF"));
	FMemory::Memcpy(WaveHeard.Format, "WAVE", strlen("WAVE"));
	WaveHeard.ChunkSize = 36 + InPCMData.Num() * sizeof(uint8);

	FMemory::Memcpy(WaveFmt.SubChunkID, "fmt ", strlen("fmt "));
	WaveFmt.SubChunkSize = 16;
	WaveFmt.AudioFormat = 1;
	WaveFmt.NumChannel = 1;
	WaveFmt.SampleRate = 16000;
	WaveFmt.BitsForSample = 16;
	WaveFmt.ByteRate = 16000 * 1 * 16 / 8;
	WaveFmt.BlockAlign = 1 * 16 / 8;

	FMemory::Memcpy(WaveData.DataChunkID, "data", strlen("data"));
	WaveData.DataChunkSize = InPCMData.Num() * sizeof(uint8);


	OutWaveData.AddUninitialized(sizeof(FXGWaveHeard));
	FMemory::Memcpy(OutWaveData.GetData(), &WaveHeard, sizeof(FXGWaveHeard));

	int32 Index = OutWaveData.AddUninitialized(sizeof(FXGWaveFmt));
	FMemory::Memcpy(&OutWaveData[Index], &WaveFmt, sizeof(FXGWaveFmt));


	Index = OutWaveData.AddUninitialized(sizeof(FXGWaveData));
	FMemory::Memcpy(&OutWaveData[Index], &WaveData, sizeof(FXGWaveData));

	OutWaveData.Append(InPCMData);



}
