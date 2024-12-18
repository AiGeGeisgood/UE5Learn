#include "XGSampleWebPMultiShotSubsystem.h"
#include "XGSampleWebPCore.h"

#include "Async/Async.h"
#include "Engine/World.h"
#include "RenderingThread.h"
#include "Slate/SceneViewport.h"
#include "Engine/GameViewportClient.h"


bool UXGSampleWebPMultiShotSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGSampleWebPMultiShotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SampleWebPMultiCallBack.BindUFunction(this, "SampleWebPOneMultiCallBackMethod");
	WebpPictureInformation = MakeShareable(new FXGSampleWebpPictureInformation);


}

void UXGSampleWebPMultiShotSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SampleWebPMultiCallBack.Unbind();
	ResetRecord();


}

void UXGSampleWebPMultiShotSubsystem::Tick(float DeltaTime)
{
	if (ProcessType == EXGSampleWebpProcessType::Recording)
	{
		RecordOneFrame(DeltaTime);
	}


}

bool UXGSampleWebPMultiShotSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId UXGSampleWebPMultiShotSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGSampleWebPMultiShotSubsystem, STATGROUP_Tickables);
}

bool UXGSampleWebPMultiShotSubsystem::BeginRecord(FString& InGeneratedWebpPicturesPath, TSharedPtr<FXGSampleWebpPictureInformation> InWebpPictureInformation)
{
	if (!GetWorld())
	{
		return false;
	}

	if (ProcessType != EXGSampleWebpProcessType::None)
	{
		return false;
	}

	if (!FXGSampleWebPCore::CheckInWebpPictureSize(GetWorld(), InWebpPictureInformation))
	{
		return false;
	}
	ResetRecord();
	ProcessType = EXGSampleWebpProcessType::Recording;
	GeneratedWebpPicturesPath = InGeneratedWebpPicturesPath;
	WebpPictureInformation = InWebpPictureInformation;
	FlushRenderingCommands();



	ScreenHandle = UGameViewportClient::OnScreenshotCaptured().AddUObject(this,
		&UXGSampleWebPMultiShotSubsystem::ScreenShotCallback);

	return true;
}

void UXGSampleWebPMultiShotSubsystem::RecordOneFrame(float DeltaTime)
{
	FScreenshotRequest::RequestScreenshot(false);

	//可能有潜在的问题
	if (WebpTimestepMillisecond.Num() == 0)
	{
		WebpTimestepMillisecond.Add(0);
	}
	else
	{
		WebpTimestepMillisecond.Add(DeltaTime * 1000);
	}



}

void UXGSampleWebPMultiShotSubsystem::EndRecord(FXGWebpFinishGenerateMultiWebp& InFinshWebpBPDelegate)
{
	if (ProcessType != EXGSampleWebpProcessType::Recording)
	{
		InFinshWebpBPDelegate.ExecuteIfBound(false);
		return;
	}
	ProcessType = EXGSampleWebpProcessType::Generating;

	FinshWebpBPDelegate = InFinshWebpBPDelegate;

	AsyncTask(ENamedThreads::AnyThread, [&]()
		{
			FPlatformProcess::Sleep(0.2);
			FScopeLock XGLock(&XGWebpMutex);

			bool GenerateWebp = FXGSampleWebPCore::GenerateDynamicWebpPicture(
				GeneratedWebpPicturesPath,
				WebpPictureInformation,
				WebPColor,
				WebpTimestepMillisecond
			);

			AsyncTask(ENamedThreads::GameThread, [&, GenerateWebp]() {

				SampleWebPMultiCallBack.ExecuteIfBound(GenerateWebp);

				});

		});


}

void UXGSampleWebPMultiShotSubsystem::ResetRecord()
{
	ProcessType = EXGSampleWebpProcessType::None;
	if (ScreenHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	}

	GeneratedWebpPicturesPath = TEXT("");
	WebpPictureInformation->ResetPictureInformation();
	WebPColor.Empty();
	WebpTimestepMillisecond.Empty();


}

void UXGSampleWebPMultiShotSubsystem::ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors)
{

	AsyncTask(ENamedThreads::AnyThread, [&, InWidth, InHeight, InColors]()
		{
			FScopeLock XGLock(&XGWebpMutex);
			TArray<FColor> OutColors;


			//截取特定像素
			for (int32 IndexY = 1; IndexY <= InHeight; IndexY++)
			{
				for (int32 IndexX = 1; IndexX <= InWidth; IndexX++)
				{
					int32 IndexXY = (IndexY - 1) * InWidth + IndexX;

					int32 IndexArray = IndexXY - 1;

					bool  bIndexX = (IndexX >= WebpPictureInformation->X0 + 1) && (IndexX <= WebpPictureInformation->X1 + 1);
					bool  bIndexY = (IndexY >= WebpPictureInformation->Y0 + 1) && (IndexY <= WebpPictureInformation->Y1 + 1);
					if (bIndexX && bIndexY)
					{
						OutColors.Add(InColors[IndexArray]);
					}
				}
			}

			//添加截取之后的该图片数据
			if (OutColors.Num() == (WebpPictureInformation->X1 - WebpPictureInformation->X0 + 1) * (WebpPictureInformation->Y1 - WebpPictureInformation->Y0 + 1))
			{
				WebPColor.Add(OutColors);
				return;
			}


		});




}

void UXGSampleWebPMultiShotSubsystem::SampleWebPOneMultiCallBackMethod(bool bGenerateWebp)
{
	if (ScreenHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	}


	ResetRecord();

	FinshWebpBPDelegate.ExecuteIfBound(bGenerateWebp);




}

