// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGSamplePictureActionAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGSamplePictureDelegate, FGuid, AsyncID, bool, bResult, FString, Message, const TArray<uint8>&, OutData);

class UTextureRenderTarget2D;

UCLASS(meta = (HideThen = true))
class XGSAMPLEPICTURE_API UXGSamplePictureActionAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UXGSamplePictureActionAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGSamplePictureActionAction();


public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XGPictureReadRenderTarget2DToPNGBinaryData",
		Keywords = "XG Picture Read RenderTarget2D  PNG"),
		Category = "XGPicture")
	static UXGSamplePictureActionAction* XGSamplePictureActionAction(UObject* WorldContextObject,
		UTextureRenderTarget2D* InRenderTarget2D,
		bool bSaveLocalTempPNG);


public:

	virtual void Activate() override;

	virtual void Activate_Internal();

protected:

	void RealeaseResources();

	void CallOnSuccess(FString InMessage, TArray<uint8> InPNGData);

	void CallOnFail(FString InMessage);


public:

	UPROPERTY(BlueprintAssignable)
	FXGSamplePictureDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGSamplePictureDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGSamplePictureDelegate OnFail;


public:

	FGuid AsyncID = FGuid::NewGuid();

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget2DPtr = nullptr;


	bool bSaveLocalTempPNG = false;

};

