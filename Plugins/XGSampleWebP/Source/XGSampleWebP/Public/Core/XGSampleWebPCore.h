// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XGSampleWebPType.h"

class XGSAMPLEWEBP_API FXGSampleWebPCore
{

public:

	//外部调用,外部必须在其他线程使用该接口,否则会阻塞主线程
	static bool GenerateStaticWebpPicture(const FString& InPicturePath,
		const	TArray<FColor>& InPictureColors,
		const FVector2D& InPictureSize,
		int32           InQualityFactor = 100);

	static bool GenerateDynamicWebpPicture(
		FString& InPicturePath,
		TSharedPtr<FXGSampleWebpPictureInformation> InWebpPictureInformation,
		TArray<TArray<FColor>>& InPicturesColors,
		TArray<int32>& WebpTimestepMillisecond,
		int32                   InQualityFactor = 100);


	static bool LoadDynamicWebpPicture
	(
		FString InWebpFilePath,
		TArray<int32>& OutWebpTimestepMillisecond,
		TArray<TArray<FColor>>& OutPicturesColors,
		int32& OutWebpWidth,
		int32& OutWebpHeight
	);





public:

	static	bool CheckWebpPicturePath(const FString& InGeneratedWebpPicturesPath) ;

	static	bool GetViewportSize(UObject* WorldContextObject, FVector2D& OutViewportSize);

	static	bool CheckInWebpPictureSize(UObject* WorldContextObject, TSharedPtr<FXGSampleWebpPictureInformation> InWebpPictureInformation);


};