#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "XGSampleSTTReqType.h"
#include "XGSampleSTTType.h"

#include "XGSampleXFLinkBPLibrary.generated.h"


UCLASS()
class XGSAMPLEXFLINK_API UXGSampleXFLinkBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XGSampleBeginRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT Soeech To Text RealTime Begin"), Category = "XGSampleLink|XF|STT")
	static 	void XGBeginRealTimeSpeechToText(UObject* WorldContextObject,
		FString InAppID,
		FString InAPIKey,
		FXGSampleSTTReqInfo InSTTReqInfo,
		FXGSampleInitSTTDelegate InSTTInitDelegate,
		FXGSampleSTTRespDelegate InSTTRespDelegate,
		FXGSampleSTTCloseDelegate InSTTCloseDelegate,
		FGuid& OutSTTAsyncID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XGSampleStopRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT Soeech To Text RealTime Stop"), Category = "XGSampleLink|XF|STT")
	static void XGStopRealTimeSpeechToText(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XGSampleForceToStopRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT Soeech To Text RealTime Force To Stop"), Category = "XGSampleLink|XF|STT")
	static void XGForceToStopRealTimeSPeechToText(const UObject* WorldContextObject);

	UFUNCTION(Blueprintpure, meta = (DisplayName = "XGSampleIsRealTimeSpeechToTextWorking", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT Soeech To Text Workin"), Category = "XGSampleLink|XF|STT")
	static bool XGIsRealTimeSpeechToTextWorking(const UObject* WorldContextObject);

	UFUNCTION(Blueprintpure, meta = (DisplayName = "XGSampleGetRealTimeSpeechToTextVolumeSize", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT Speech To Text RealTime Volume Size"), Category = "XGSampleLink|XF|STT")

	static float XGXunFeiGetRealTimeSpeechToTextVolumeSize(UObject* WorldContextObject);



};