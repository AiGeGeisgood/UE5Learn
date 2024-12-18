#pragma once

#include "CoreMinimal.h"
#include "XGSampleTTSRespType.generated.h"



USTRUCT()
struct XGSAMPLEXFLINK_API FXGXunFeiTTSRespInfoData
{
	GENERATED_BODY()

public:



	UPROPERTY()
	FString audio = TEXT("");


	UPROPERTY()
	int32 status = -1;

	UPROPERTY()
	FString ced = TEXT("");
};




USTRUCT()
struct XGSAMPLEXFLINK_API FXGXunFeiTTSRespInfo
{
	GENERATED_BODY()

	friend class UXGXunFeiTTSAsyncAction;

public:

	UPROPERTY()
	int32 code = -1;

	UPROPERTY()
	FString message = TEXT("");

	UPROPERTY()
	FXGXunFeiTTSRespInfoData data;

	UPROPERTY()
	FString sid = TEXT("");

};
