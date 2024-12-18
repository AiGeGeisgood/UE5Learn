#include "XGSampleWebPOneShotSubsystem.h"
#include "XGSampleWebPCore.h"

#include "Async/Async.h"
#include "Engine/World.h"
#include "RenderingThread.h"
#include "Slate/SceneViewport.h"
#include "Engine/GameViewportClient.h"
#include "Misc/Paths.h"

bool UXGSampleWebPOneShotSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGSampleWebPOneShotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SampleWebPOneShotCallBack.BindUFunction(this, "SampleWebPOneShotCallBackMethod");


}

void UXGSampleWebPOneShotSubsystem::Deinitialize()
{
	SampleWebPOneShotCallBack.Unbind();
	Super::Deinitialize();


}

void UXGSampleWebPOneShotSubsystem::BeginSampleWebPOneShot(FSampleWebPOneShotCallBackBP InSampleWebPOneShotCallBackBP)
{

	if (bWorking)
	{
		InSampleWebPOneShotCallBackBP.ExecuteIfBound(false);

		return ;
	}

	bWorking =true;

	SampleWebPOneShotCallBackBP = InSampleWebPOneShotCallBackBP;

	FlushRenderingCommands();

	ScreenHandle = UGameViewportClient::OnScreenshotCaptured().AddUObject(this,
		&UXGSampleWebPOneShotSubsystem::ScreenShotCallback);

	FScreenshotRequest::RequestScreenshot(false);



}

void UXGSampleWebPOneShotSubsystem::SampleWebPOneShotCallBackMethod(bool bGenerateWebp)
{
	bWorking = false;




	SampleWebPOneShotCallBackBP.ExecuteIfBound(bGenerateWebp);

}

void UXGSampleWebPOneShotSubsystem::ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors)
{
	if (ScreenHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	}


	AsyncTask(ENamedThreads::AnyThread, [this,InWidth, InHeight, InColors]() {

		FString FileName = FGuid::NewGuid().ToString() + TEXT("_OneShot.webp");

		FString PictruePath = FPaths::ProjectSavedDir() / FileName;

		FVector2D PSize;

		PSize.X = InWidth;

		PSize.Y = InHeight;
		bool bFinishWebp = FXGSampleWebPCore::GenerateStaticWebpPicture(PictruePath, InColors, PSize, 100);

		//一定要是回调到主线程
		AsyncTask(ENamedThreads::GameThread, [this, bFinishWebp]() {

			SampleWebPOneShotCallBack.ExecuteIfBound(bFinishWebp);

			});

	


		});



}
