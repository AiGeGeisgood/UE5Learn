#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGSampleTTSReqType.h"
#include "XGSampleTTSRespType.h"

#include "XGSampleTTSAsyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGXunFeiTTSDelegate, FGuid, AsyncID, bool, bResult, FString, Message, USoundWave*, SoundWavePtr);


// 头信息 格式+总数据大小
// 格式描述信息 关于数据格式的描述
// 数据信息描述 +数据大小
//  真正的数据
//12个字节
struct FXGWaveHeard
{
	//4个字节
	int8 ChunkID[4];
	//4个字节 36+数据大小 不包含 ChunkID  ChunkSize 4+ 24 +8 =36
	uint32 ChunkSize;
	//4个字节 
	int8 Format[4];
};


//24个字节
struct FXGWaveFmt
{
	//4个字节 
	int8 SubChunkID[4];
	//4个字节 16字节 不包含SubChunkID  SubChunkSize 2+2+4+4+2+2 =16
	uint32 SubChunkSize;
	//2个字节 1 
	uint16 AudioFormat;
	//2个字节 1
	uint16 NumChannel;
	//4个字节 16000
	uint32 SampleRate;
	//4个字节 byte
	uint32 ByteRate;
	//2个字节  2个字节对齐
	uint16 BlockAlign;
	//2个字节  16位
	uint16 BitsForSample;



};

//8个字节
struct FXGWaveData
{
	//4个字节
	int8 DataChunkID[4];
	//4个字节
	uint32 DataChunkSize;

};
//追加二进制数据




class IWebSocket;
class USoundWave;

UCLASS(meta = (HideThen = true))
class XGSAMPLEXFLINK_API UXGSampleTTSAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UXGSampleTTSAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGSampleTTSAsyncAction();

	/**
	 * XGXunFeiTextToSpeech BlueprintNode
	 * If you want to use in C++,please see XGXunFeiLinkBPLibrary.h .
	 *
	 * @param WorldContextObject		WorldContext
	 *
	 * @param InTTSAppID				iFlyTek AppID
	 * @param InTTSAPISecret			iFlyTek APISecret
	 * @param InTTSAPIKey				iFlyTek APIKey
	 *
	 * @param InText					The Str you want to Convert to Audio
	 * @param bInSaveToLocal			Whether to Save to local disk
	 * @param InSaveFileFullPath		The FilePath which the wmv file will save to .
	 *									This directory must exist,and the file name must end with ".wav"
	 *									This path is absoult path!
	 * @param InXunFeiTTSReqInfo		About TTS Settins,more to look iflytesk document.you can choose which voice to say,and so on.
	 *
	 * @return UXGXunFeiTTSAsyncAction*	Self object ptr
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XunFeiTextToSpeech",
		Keywords = "XG Sample TTS Text To Speech "),
		Category = "XGSampleLink|TTS|TTS")
	static UXGSampleTTSAsyncAction* XGXunFeiTextToSpeech(UObject* WorldContextObject,
		FString InTTSAppID,
		FString InTTSAPISecret,
		FString InTTSAPIKey,
		const FString& InText,
		bool bInSaveToLocal,
		const FString& InSaveFileFullPath,
		FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo);

public:

	virtual void Activate() override;

	virtual void Activate_Internal();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "XunFeiLoadFile",Category = "XGSampleLink|TTS|TTS"))
	static bool LoadLocalFileData(FString InPath,TArray<uint8>& OutBinaryData);


protected:

	void RealeaseResources();

	void CreateXunFeiTTSSocket();

	void CloseXunFeiTTsSocket();

	void ForceToCloseXunFeiTTsSocket();


	FORCEINLINE void SetXunFeiText(const FString& InXunFeiText) { XunFeiText = InXunFeiText; }
	FORCEINLINE void SetIsSaveToLocal(bool bInSaveToLocal) { bSaveToLocal = bInSaveToLocal; }
	FORCEINLINE void SetSaveFileFullPath(const FString& InSaveFilePath) { SaveFileFullPath = InSaveFilePath; }
	FORCEINLINE	void SetXunFeiTTSReqInfo(const FXGXunFeiTTSReqInfo& InXunFeiTTSReqInfo) { XunFeiTTSReqInfo = InXunFeiTTSReqInfo; }

protected:


	void OnConnected();

	void OnConnectionError(const FString& ErrorMessage);

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	void OnMessage(const FString& Message);

	void OnMessageSent(const FString& MessageString);



	void CallOnSoundWaveSuccess(bool bInResult, FString InMessage, USoundWave* InSoundWavePtr);


	void CallOnSoundWaveFail(bool bInResult, FString InMessage, USoundWave* InSoundWavePtr = nullptr);


	void CallGenerateWavFile();


protected:

	//把一个虚幻引擎的字符串转换为UTF-8编码格式,再对转换后的字符串进行Base64加密
	FString TCharStringToUTF8Base64String(const FString& InTCharString);

	//把一段经过Base64j加密后的UTF8字符串解密成虚幻引擎字符串
	static FString Base64UTF8StringToTCharString(const FString& InBase64UTF8String);

	static USoundWave* ImportPCMToSoundWave(const TArray<uint8>& InPCMData);


	static USoundWave* ImportPCMToSoundWave_Method1(const TArray<uint8>& InPCMData);

	static USoundWave* ImportPCMToSoundWave_Method2(const TArray<uint8>& InPCMData);


	static void ConvertPCMToWave(const TArray<uint8>& InPCMData, TArray<uint8>& OutWaveData);







public:

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate OnSoundWaveSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate OnSoundWaveFail;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate OnWavFileSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate OnWavFileFail;


protected:

	FGuid AsyncID = FGuid::NewGuid();

	FString AppID = TEXT("");

	FString APISecret = TEXT("");

	FString APIKey = TEXT("");

	bool bSaveToLocal = false;

	FString SaveFileFullPath = TEXT("");

	FXGXunFeiTTSReqInfo XunFeiTTSReqInfo;

	TSharedPtr<IWebSocket> Socket;

	TArray<uint8> XunFeiAudioData;

	FString XunFeiText =TEXT("");

};