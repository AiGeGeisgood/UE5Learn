#pragma once

#include "CoreMinimal.h"
#include "XGSampleXFSilentBiopsyReqType.generated.h"

UENUM(BlueprintType)
enum class EXGSampleXFSilentBiopsyImgTpye :uint8
{
	Jpg UMETA(DisplayName = "JPG"),
	Jpeg UMETA(DisplayName = "JPEG"),
	Png UMETA(DisplayName = "PNG"),
	Bmp UMETA(DisplayName = "BMP"),
};


#pragma region Header

USTRUCT()
struct FXGSampleXFSilentBiopsyReqHeaderInfo
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString app_id = TEXT("");
	UPROPERTY()
	int32 status = 3;
};
#pragma endregion Header

#pragma region Parameter


USTRUCT()
struct  FXGSampleXFSilentBiopsyReqParameterAntiSpoofResultInfo
{
	GENERATED_BODY()


public:


	UPROPERTY()
	FString encoding = TEXT("utf8");


	UPROPERTY()
	FString compress = TEXT("raw");



	UPROPERTY()
	FString format = TEXT("json");

};

USTRUCT()
struct FXGSampleXFSilentBiopsyReqParameterS67c9c78cInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString service_kind = TEXT("anti_spoof");
	UPROPERTY()
	FXGSampleXFSilentBiopsyReqParameterAntiSpoofResultInfo anti_spoof_result;


};

USTRUCT()
struct FXGSampleXFSilentBiopsyReqParameterInfo
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FXGSampleXFSilentBiopsyReqParameterS67c9c78cInfo s67c9c78c;

};

#pragma endregion Parameter

#pragma region Payload


USTRUCT()
struct FXGSampleXFBiopsyReqPayloadInputInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString encoding = TEXT("jpg");
	UPROPERTY()
	int32 status = 3;
	UPROPERTY()
	FString image = TEXT("");

};

USTRUCT()
struct FXGSampleXFSilentBiopsyReqPayloadInfo
{
	GENERATED_BODY()

	friend class UXGXunFeiSilentBiopsyAsyncAction;

public:
	UPROPERTY()
	FXGSampleXFBiopsyReqPayloadInputInfo input1;

};

#pragma endregion Payload
USTRUCT(BlueprintType)
struct FXGSampleXFSilentBiopsyReqInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	FXGSampleXFSilentBiopsyReqHeaderInfo header;

	UPROPERTY()
	FXGSampleXFSilentBiopsyReqParameterInfo parameter;

	UPROPERTY()
	FXGSampleXFSilentBiopsyReqPayloadInfo payload;


};