// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Engine/Texture2D.h"
#include "XGSampleWebPType.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FXGWebpFinishGenerateMultiWebp, bool, bFinishGenerate);


DECLARE_DYNAMIC_DELEGATE_FourParams(FXGWebpLoadAndShowWebp, bool, bLoad, UTexture2D*, OutWebpPicture, int32, WebpWidth, int32, WebpHeight);

USTRUCT(BlueprintType)
struct XGSAMPLEWEBP_API FXGSampleWebpPictureInformation
{
	GENERATED_USTRUCT_BODY()

public:
	FXGSampleWebpPictureInformation();
	FXGSampleWebpPictureInformation(int32 InX0, int32 InY0, int32 InX1, int32 InY1);
	FXGSampleWebpPictureInformation(FXGSampleWebpPictureInformation& InWebpPictureInformation);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGWebp")
	int32 X0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGWebp")
	int32 Y0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGWebp")
	int32 X1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGWebp")
	int32 Y1;

	int32 GetPictureWidth();
	int32 GetPictureHeight();

	void ResetPictureInformation();


};

UENUM()
enum class EXGSampleWebpProcessType :uint8
{
	None,
	Recording,
	Generating,
	Max
};



UENUM()
enum class EXGSampleWebpLoadAndShowType :uint8
{
	None,
	Loading,
	Showing,
	Max
};