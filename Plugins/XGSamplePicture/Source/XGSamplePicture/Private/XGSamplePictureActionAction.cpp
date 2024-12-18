#include "XGSamplePictureActionAction.h"

#include "Engine/TextureDefines.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RHITypes.h"
#include "TextureResource.h"
#include "ImageUtils.h"

#include "Async/Async.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogXGSamplePicture, Display, All);

UXGSamplePictureActionAction::UXGSamplePictureActionAction(const FObjectInitializer& ObjectInitializer)
{
}

UXGSamplePictureActionAction::~UXGSamplePictureActionAction()
{

	RealeaseResources();
}

UXGSamplePictureActionAction* UXGSamplePictureActionAction::XGSamplePictureActionAction(UObject* WorldContextObject,
	UTextureRenderTarget2D* InRenderTarget2D,
	bool bSaveLocalTempPNG)
{
	check(WorldContextObject);

	UXGSamplePictureActionAction* AsyncAction = NewObject<UXGSamplePictureActionAction>();

	AsyncAction->RenderTarget2DPtr = InRenderTarget2D;
	AsyncAction->bSaveLocalTempPNG = bSaveLocalTempPNG;

	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGSamplePictureActionAction::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});

	Then.Broadcast(AsyncID, false, TEXT("XGSamplePictureActionAction is just started,please wait to finish"), {});


}

void UXGSamplePictureActionAction::Activate_Internal()
{

	if (RenderTarget2DPtr == nullptr)
	{
		FString ErrorMessage = TEXT("UTextureRenderTarget2D is nullptr");

		UE_LOG(LogXGSamplePicture, Error, TEXT("[%s],AsyncID:[%s],Message:[%s]"), *FString(__FUNCTION__), *AsyncID.ToString(), *ErrorMessage);

		CallOnFail(ErrorMessage);

		RealeaseResources();

		return;

	}

	FTextureRenderTargetResource* RenderTargetResource = RenderTarget2DPtr->GameThread_GetRenderTargetResource();
	int32 Height = RenderTarget2DPtr->GetSurfaceHeight();
	int32 Wide = RenderTarget2DPtr->GetSurfaceWidth();
	int32 PixelNum = Height * Wide;

	FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);

	readPixelFlags.SetLinearToGamma(true);

	TArray<FColor> OutColors;

	OutColors.AddUninitialized(PixelNum);

	bool bRead = RenderTargetResource->ReadPixels(OutColors, readPixelFlags);

	if (!bRead)
	{
		FString ErrorMessage = TEXT("Read Error");
		UE_LOG(LogXGSamplePicture, Error, TEXT("[%s],AsyncID:[%s],Message:[%s]"), *FString(__FUNCTION__), *AsyncID.ToString(), *ErrorMessage);
		CallOnFail(ErrorMessage);

		RealeaseResources();

		return;
	}

	for (auto& TmpColor : OutColors)
	{
		TmpColor.A = 255 - TmpColor.A;
	}

	AsyncTask(ENamedThreads::AnyThread, [Wide, Height, OutColors, this]()
		{

			TArray<uint8> OutPictureData;

			TArray64<uint8> PictureData;
			FImageUtils::PNGCompressImageArray(Wide, Height, OutColors, PictureData);

			OutPictureData = PictureData;

			if (bSaveLocalTempPNG)
			{
				FString FilePath = FPaths::ConvertRelativePathToFull((FPaths::ProjectSavedDir())) / TEXT("TestVideo.png");
				FFileHelper::SaveArrayToFile(OutPictureData, *FilePath);
			}

			AsyncTask(ENamedThreads::GameThread, [OutPictureData, this]()
				{

					FString SucceedMessage = TEXT("Generate PNG Data Finish");

					UE_LOG(LogXGSamplePicture, Verbose, TEXT("[%s],AsyncID:[%s],Message:[%s]"), *FString(__FUNCTION__), *AsyncID.ToString(), *SucceedMessage);


					CallOnSuccess(TEXT("Generate Succeed"), OutPictureData);


					RealeaseResources();

				});



		});




}

void UXGSamplePictureActionAction::RealeaseResources()
{
	AsyncID = FGuid();

	RenderTarget2DPtr = nullptr;

	bSaveLocalTempPNG = false;

	SetReadyToDestroy();
}

void UXGSamplePictureActionAction::CallOnSuccess(FString InMessage, TArray<uint8> InPNGData)
{

	FXGSamplePictureDelegate TempDelegate = OnSuccess;

	FGuid TempGuid = AsyncID;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(TempGuid, true, InMessage, InPNGData);

		});

}

void UXGSamplePictureActionAction::CallOnFail(FString InMessage)
{
	FXGSamplePictureDelegate TempDelegate = OnFail;

	FGuid TempGuid = AsyncID;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(TempGuid, false, InMessage, {});

		});
}
