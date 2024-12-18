#pragma once

#include "CoreMinimal.h"

#include "XGSampleXFSilentBiopsyRespType.generated.h"

#pragma region Header

USTRUCT(BlueprintType)
struct XGSAMPLEXFLINK_API FXGSampleXFSilentBiopsyRespHeaderInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	int32 code = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString message = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString sid = TEXT("");

};

#pragma endregion Header

#pragma region Payload

USTRUCT(BlueprintType)
struct XGSAMPLEXFLINK_API FXGSampleXFSilentBiopsyRespPayloadResultInfo
{
	GENERATED_USTRUCT_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString compress = TEXT("");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString encoding = TEXT("");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString format = TEXT("");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString text = TEXT("");

};


USTRUCT(BlueprintType)
struct XGSAMPLEXFLINK_API FXGSampleXFBiopsyRespPayloadInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FXGSampleXFSilentBiopsyRespPayloadResultInfo anti_spoof_result;
};







#pragma endregion Payload

USTRUCT(BlueprintType)
struct XGSAMPLEXFLINK_API FXGSampleXFSilentBiopsyRespTextMessage
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	int32 ret = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	bool passed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	float score = -1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	int32 x = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	int32 y = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	int32 w = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	int32 h = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FString thresholds = TEXT("");


};



USTRUCT(BlueprintType)
struct XGSAMPLEXFLINK_API FXGSampleXFSilentBiopsyRespMessage
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FXGSampleXFSilentBiopsyRespHeaderInfo header;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FXGSampleXFBiopsyRespPayloadInfo payload;

};




USTRUCT(BlueprintType)
struct XGSAMPLEXFLINK_API FXGSampleXFSilentBiopsyRespInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	bool bPassed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	float Score = -1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FXGSampleXFSilentBiopsyRespTextMessage TextMessage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek SilentBiopsy")
	FXGSampleXFSilentBiopsyRespMessage RawMessage;

};