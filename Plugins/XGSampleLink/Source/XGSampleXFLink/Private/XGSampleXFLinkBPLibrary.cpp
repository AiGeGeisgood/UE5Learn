#include "XGSampleXFLinkBPLibrary.h"




#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "XGSampleSTTSubsystem.h"
#include "XGSampleAudioCaptureSubsystem.h"

void UXGSampleXFLinkBPLibrary::XGBeginRealTimeSpeechToText(
	 UObject* WorldContextObject,
	FString InAppID,
	FString InAPIKey,
	FXGSampleSTTReqInfo InSTTReqInfo,
	FXGSampleInitSTTDelegate InSTTInitDelegate,
	FXGSampleSTTRespDelegate InSTTRespDelegate,
	FXGSampleSTTCloseDelegate InSTTCloseDelegate,
	FGuid& OutSTTAsyncID)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGSampleSTTSubsystem>();

		OutSTTAsyncID = RealTimeSTTSubsystem->XGBeginRealTimeSpeechToText(
			InAppID,
			InAPIKey,
			InSTTReqInfo,
			InSTTInitDelegate,
			InSTTRespDelegate,
			InSTTCloseDelegate);
	}
	else
	{
		OutSTTAsyncID = FGuid::NewGuid();

		AsyncTask(ENamedThreads::GameThread, [InSTTInitDelegate, OutSTTAsyncID]() {

			InSTTInitDelegate.ExecuteIfBound(OutSTTAsyncID, false, TEXT("WorldContextObject is null"), FXGSampleSTTInitInformation());

			});
	}

}

void UXGSampleXFLinkBPLibrary::XGStopRealTimeSpeechToText(const UObject* WorldContextObject)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGSampleSTTSubsystem>();

		RealTimeSTTSubsystem->XGStopRealTimeSpeechToText();

	}
	else
	{

		check(false);
	}


}

void UXGSampleXFLinkBPLibrary::XGForceToStopRealTimeSPeechToText(const UObject* WorldContextObject)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGSampleSTTSubsystem>();

		RealTimeSTTSubsystem->XGForceToStopRealTimeSPeechToText(false);

	}
	else
	{

		check(false);
	}


}

bool UXGSampleXFLinkBPLibrary::XGIsRealTimeSpeechToTextWorking(const UObject* WorldContextObject)
{

	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGSampleSTTSubsystem>();

		return RealTimeSTTSubsystem->XGIsRealTimeSpeechToTextWorking();

	}
	else
	{

		check(false);
	}


	return false;
}

float UXGSampleXFLinkBPLibrary::XGXunFeiGetRealTimeSpeechToTextVolumeSize(UObject* WorldContextObject)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGSampleAudioCaptureSubsystem* AudioCaptureSubsystem = GameInstance->GetSubsystem<UXGSampleAudioCaptureSubsystem>();

		return AudioCaptureSubsystem->GetVolumeSize();

	}
	else
	{

		check(false);
	}


	return false;


	return 0.0f;
}
