#pragma once

#include "CoreMinimal.h"
#include "XGSampleSTTRespType.h"

#include"XGSampleSTTType.generated.h"

DECLARE_DYNAMIC_DELEGATE_FourParams(FXGSampleInitSTTDelegate,
	FGuid, AsyncSTTID,
	bool, bInitResult,
	FString, InitRespMessage,
	FXGSampleSTTInitInformation, RealTimeSTTInitInformation);

DECLARE_DYNAMIC_DELEGATE_OneParam(FXGSampleSTTRespDelegate,
	FXGSampleSTTInformation, RealTimeSTTInformation);

DECLARE_DYNAMIC_DELEGATE_FourParams(FXGSampleSTTCloseDelegate,
	FGuid, AsyncSTTID,
	int32, StatusCode,
	FString, Reason,
	bool, bWasClean);




UENUM()
enum class EXGSampleSTTStatus : uint8
{
	Ready,
	Init,
	Processing,
	WaitToServerClose,
	Finish,
};
