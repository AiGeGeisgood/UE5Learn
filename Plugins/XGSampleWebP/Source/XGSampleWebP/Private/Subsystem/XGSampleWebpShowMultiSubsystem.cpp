#include "XGSampleWebpShowMultiSubsystem.h"


#include "Engine/Texture2D.h"
#include "Async/Async.h"
#include "XGSampleWebPCore.h"
#include "TextureResource.h"
#include "Engine/Texture.h"
bool UXGSampleWebpShowMultiSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGSampleWebpShowMultiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	Super::Initialize(Collection);
}

void UXGSampleWebpShowMultiSubsystem::Deinitialize()
{
	ReleaseLoadedWebp();
	Super::Deinitialize();
}

void UXGSampleWebpShowMultiSubsystem::Tick(float DeltaTime)
{
	if (LoadAndShowStatus == EXGSampleWebpLoadAndShowType::Showing && WebpTexture)
	{
		DeltaTime *= 1000;
		//TODO,cost much
		for (int32 WebpIndex = 0; WebpIndex < WebpTimestepMillisecond.Num(); WebpIndex++)
		{
			if (WebpIndex == 0 && CurrentMillisecond < WebpTimestepMillisecond[WebpIndex])
			{
				CurrentMillisecond += DeltaTime;
				WebpShowIndex = WebpIndex;
				break;
			}

			if (WebpIndex == WebpTimestepMillisecond.Num() - 1 && CurrentMillisecond > WebpTimestepMillisecond[WebpIndex])
			{
				CurrentMillisecond = 0;
				WebpShowIndex = WebpIndex;
				break;
			}

			if (CurrentMillisecond > WebpTimestepMillisecond[WebpIndex])
			{
				continue;
			}

			CurrentMillisecond += DeltaTime;
			WebpShowIndex = WebpIndex;
			break;

		}


		uint8* textureData = static_cast<uint8*>(WebpTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		FMemory::Memcpy(textureData, PicturesColors[WebpShowIndex].GetData(), WebpWidth * WebpHeight * 4);

		WebpTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		WebpTexture->UpdateResource();


	}



}

bool UXGSampleWebpShowMultiSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId UXGSampleWebpShowMultiSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGSampleWebpShowMultiSubsystem, STATGROUP_Tickables);
}

void UXGSampleWebpShowMultiSubsystem::LoadWebp(FXGWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath)
{
	bool bRightPath = FXGSampleWebPCore::CheckWebpPicturePath(InWebpFilePath);
	bool bRgihtStatus = LoadAndShowStatus == EXGSampleWebpLoadAndShowType::None;

	if (!bRightPath || !bRgihtStatus)
	{
		InLoadAndShowWebpDegelete.ExecuteIfBound(false, nullptr, 0, 0);
		return;
	}


	LoadAndShowStatus = EXGSampleWebpLoadAndShowType::Loading;

	XGWebpLoadAndShowWebp = InLoadAndShowWebpDegelete;



	AsyncTask(ENamedThreads::AnyThread,[InWebpFilePath,this](){
	
		//此处需要异步
		bool bLoadWebp = FXGSampleWebPCore::LoadDynamicWebpPicture(InWebpFilePath, WebpTimestepMillisecond, PicturesColors, WebpWidth, WebpHeight);


		//此处需要返回主线程
		AsyncTask(ENamedThreads::GameThread, [bLoadWebp, this]() {

			LoadedWebpCallShow(bLoadWebp);

			});

	
	});




}

void UXGSampleWebpShowMultiSubsystem::ReleaseLoadedWebp()
{
	switch (LoadAndShowStatus)
	{
	case EXGSampleWebpLoadAndShowType::None:
		break;
	case EXGSampleWebpLoadAndShowType::Loading:

		AsyncTask(ENamedThreads::GameThread, [this]() {

			ReleaseLoadedWebp();

			});
		
		break;
	case EXGSampleWebpLoadAndShowType::Showing:
		LoadAndShowStatus = EXGSampleWebpLoadAndShowType::None;
		WebpTexture = nullptr;
		WebpShowIndex = -1;
		CurrentMillisecond = -1;
		WebpWidth = 0;
		WebpHeight = 0;
		WebpTimestepMillisecond.Empty();

		for (auto& Tmp : PicturesColors)
		{
			Tmp.Empty();
		}
		PicturesColors.Empty();



		break;
	case EXGSampleWebpLoadAndShowType::Max:
		break;
	default:
		break;
	}
}

void UXGSampleWebpShowMultiSubsystem::LoadedWebpCallShow(bool bLoaded)
{

	if (bLoaded)
	{
		LoadAndShowStatus = EXGSampleWebpLoadAndShowType::Showing;

		WebpTexture = UTexture2D::CreateTransient(WebpWidth, WebpHeight, EPixelFormat::PF_R8G8B8A8);

		uint8* textureData = static_cast<uint8*>(WebpTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		//uint8* textureData = static_cast<uint8*>(WebpTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

		FMemory::Memcpy(textureData, PicturesColors[0].GetData(), WebpWidth * WebpHeight * 4);
		WebpShowIndex = 0;
		WebpTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		//WebpTexture->PlatformData->Mips[0].BulkData.Unlock();

		WebpTexture->UpdateResource();

		XGWebpLoadAndShowWebp.ExecuteIfBound(true, WebpTexture, WebpWidth, WebpHeight);

	}
	else
	{
		LoadAndShowStatus = EXGSampleWebpLoadAndShowType::None;
		XGWebpLoadAndShowWebp.ExecuteIfBound(false, nullptr, 0, 0);

	}


}
