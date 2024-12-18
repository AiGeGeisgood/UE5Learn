#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "XGSampleSTTReqType.h"
#include "XGSampleSTTRespType.h"
#include "XGSampleSTTType.h"

#include "XGSampleSTTSubsystem.generated.h"

class IWebSocket;
class UXGSampleConsumeVoiceRunnable;

UCLASS()
class XGSAMPLEXFLINK_API UXGSampleSTTSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class UXGSampleXFLinkBPLibrary;
	friend class FXGSampleConsumeVoiceRunnable;
	


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


protected:

	bool IsRealTimeSTTReady();

	
	FGuid XGBeginRealTimeSpeechToText(
		FString InAppID,
		FString InAPIKey,
		FXGSampleSTTReqInfo InSTTReqInfo,
		FXGSampleInitSTTDelegate InSTTInitDelegate,
		FXGSampleSTTRespDelegate InSTTRespDelegate,
		FXGSampleSTTCloseDelegate InSTTCloseDelegate);

	void XGStopRealTimeSpeechToText();

	void XGForceToStopRealTimeSPeechToText(bool IsDeinitialize = false);

	bool XGIsRealTimeSpeechToTextWorking();

	void CallInitRealTimeSTTDelegate(FGuid InSTTAsyncID, bool bInInitResult, FString InInitRespMessage, FXGSampleSTTInitInformation InSTTInitInformation);

	void CallRealTimeSTTRespDelegate(FXGSampleSTTInformation  InSTTInformation);

	void CallRealTimeSTTCloseDelegate(FGuid InSTTAsyncID, int32 InStatusCode, FString InReason, bool bInWasClean);

	void RealseaseVoiceGenerateRunnale();

	void RealeaseVoiceConsumeRunnable();

	void RealeaseWebSocket();

	void RealeaseSTTInfo();

	void SendVoiceData(const TArray<float>& InVoiceData);

	void EndSendVoiceData();


protected:

	void OnConnected();

	void OnConnectionError(const FString& ErrorMessage);

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	void OnMessage(const FString& Message);

	void OnMessageSent(const FString& MessageString);



protected:

	static UXGSampleSTTSubsystem* RealTimeSTTSubsystemPtr;

	EXGSampleSTTStatus STTStatus = EXGSampleSTTStatus::Ready;

	FGuid AsyncID = FGuid();

	FString AppID = TEXT("");

	FString APIKey = TEXT("");

	FXGSampleInitSTTDelegate STTInitDelegate;

	FXGSampleSTTRespDelegate STTRespDelegate;

	FXGSampleSTTCloseDelegate STTCloseDelegate;

	TSharedPtr<IWebSocket> Socket;

	TSharedPtr<FXGSampleConsumeVoiceRunnable> ConsumeVoiceRunnable;

	FRunnableThread* RunnableThread;

};