// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XGSampleWebPType.h"

#include "XGSampleWebPBPLibrary.generated.h"


UCLASS()
class XGSAMPLEWEBP_API UXGSampleWebPBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAuth", Keywords = "Set XG Webp Auth"), Category = "XGSampleWebp")
	static void SetXGWebpAuth();


	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetXGSampleWebpVersion", Keywords = "Get XG Sample Webp Version"), Category = "XGSampleWebp")
	static bool GetXGSampleWebpVersion(FString& OutVersionInfo);


public:

	UFUNCTION(BlueprintCallable, Category = "XGSampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "BeginRecord"))
	static	void BeginRecord(
		UObject* WorldContextObject,
		FString InGeneratedWebpPicturesPath,
		FXGSampleWebpPictureInformation InWebpPictureInformation,
		bool& bBegin);


	UFUNCTION(BlueprintCallable, Category = "XGSampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "BeginRecordFullViewport"))
	static	void BeginRecordFullViewport(
		UObject* WorldContextObject,
		FString InGeneratedWebpPicturesPath,
		bool& bBegin);


	UFUNCTION(BlueprintCallable, Category = "XGSampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "EndRecord"))
	static	void EndRecord(UObject* WorldContextObject, FXGWebpFinishGenerateMultiWebp InFinishWebpBPDegelete);


	UFUNCTION(BlueprintCallable, Category = "XGSampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "LoadWebp"))
	static	void LoadWebp(UObject* WorldContextObject, FXGWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath);

	UFUNCTION(BlueprintCallable, Category = "XGWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "ReleaseLoadedWebp"))
	static void ReleaseLoadedWebp(UObject* WorldContextObject);






protected:

	static bool bAuth;



};
