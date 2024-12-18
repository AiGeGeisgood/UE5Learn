#include "XGSampleWebPCore.h"

#include "XGSampleWebPLib.h"
#include "LogXGSampleWebP.h"


#include "Slate/SceneViewport.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Misc/Paths.h"



bool FXGSampleWebPCore::GenerateStaticWebpPicture(const FString& InPicturePath,
	const TArray<FColor>& InPictureColors,
	const FVector2D& InPictureSize,
	int32 InQualityFactor)
{

	if (!CheckWebpPicturePath(InPicturePath))
	{
		return false;
	}


	if (InPictureColors.Num() != InPictureSize.X * InPictureSize.Y)
	{
		UE_LOG(LogXGSampleWebP, Display, TEXT("[%s] :Wrong ColorsNum"), *FString(__FUNCTION__));
		return false;
	}

	//这里的写法比较危险
	//路径不能有中文,路径过长会被截断,这断字符串指针很快就会被回收
	const char* OutPicturePath = TCHAR_TO_ANSI(*InPicturePath);

	unsigned char* Data = new unsigned char[InPictureColors.Num() * 4 + 1];

	memset(Data, 0, InPictureColors.Num() * 4 + 1);

	int i = 0;

	//A通道需要反转

	for (auto& Tmp : InPictureColors)
	{
		Data[i] = Tmp.R;
		i++;
		Data[i] = Tmp.G;
		i++;
		Data[i] = Tmp.B;
		i++;
		Data[i] = Tmp.A;
		i++;
	}

	InQualityFactor = FMath::Clamp(InQualityFactor, 0, 100);


	bool Result = FXGSampleWebPLib::GenerateWebpByRGBA(OutPicturePath,
		Data,
		InPictureSize.X,
		InPictureSize.Y,
		InQualityFactor);

	delete[] Data;
	Data = nullptr;

	return Result;
}

bool FXGSampleWebPCore::GenerateDynamicWebpPicture(FString& InPicturePath,
	TSharedPtr<FXGSampleWebpPictureInformation> InWebpPictureInformation,
	TArray<TArray<FColor>>& InPicturesColors,
	TArray<int32>& WebpTimestepMillisecond,
	int32 InQualityFactor)
{
	if (!CheckWebpPicturePath(InPicturePath))
	{
		return false;
	}

	for (auto& InPictureColors : InPicturesColors)
	{
		if (InPictureColors.Num() != InWebpPictureInformation->GetPictureWidth() * InWebpPictureInformation->GetPictureHeight())
		{
			return false;
		}
	}

	if (InPicturesColors.Num() != WebpTimestepMillisecond.Num())
	{
		return false;
	}

	const char* OutPicturePath = TCHAR_TO_ANSI(*InPicturePath);

	std::vector<const unsigned char*> Datas;

	for (auto& InPictureColors : InPicturesColors)
	{
		unsigned char* Data = new unsigned char[InPictureColors.Num() * 4 + 1];
		memset(Data, 0, InPictureColors.Num() * 4 + 1);
		int i = 0;

		for (auto& Tmp : InPictureColors)
		{
			Data[i] = Tmp.R;
			i++;
			Data[i] = Tmp.G;
			i++;
			Data[i] = Tmp.B;
			i++;
			Data[i] = Tmp.A;
			i++;
		}

		Datas.push_back(Data);
	}


	std::vector<int> Timestamps;
	for (auto& InTimestamp : WebpTimestepMillisecond)
	{
		Timestamps.push_back(InTimestamp);
	}
	InQualityFactor = FMath::Clamp(InQualityFactor, 0, 100);


	bool Result = FXGSampleWebPLib::GenerateDymaicWebpByRGBA(
		OutPicturePath,
		Datas,
		Timestamps,
		InWebpPictureInformation->GetPictureWidth(),
		InWebpPictureInformation->GetPictureHeight(),
		InQualityFactor);

	for (auto& Data : Datas)
	{
		delete[] Data;
		Data = nullptr;
	}



	return Result;
}

bool FXGSampleWebPCore::LoadDynamicWebpPicture(FString InWebpFilePath,
TArray<int32>& OutWebpTimestepMillisecond, 
TArray<TArray<FColor>>& OutPicturesColors,
int32& OutWebpWidth, 
int32& OutWebpHeight)
{
	if (!CheckWebpPicturePath(InWebpFilePath))
	{
		return false;
	}

	OutWebpTimestepMillisecond.Empty();
	OutPicturesColors.Empty();

	//这里的写法比较危险
	//路径不能有中文,路径过长会被截断,这断字符串指针很快就会被回收
	const char* InWebpFilePath_ANSI = TCHAR_TO_ANSI(*InWebpFilePath);

	std::vector<const unsigned char*> OutRGBADatas;
	std::vector<int> OutTimestamps_ms;


	bool bLoadWebp = FXGSampleWebPLib::LoadDynamicWebpPictureByRGBA(InWebpFilePath_ANSI, OutRGBADatas, OutTimestamps_ms, OutWebpWidth, OutWebpHeight);

	if (!bLoadWebp)
	{
		return false;

	}

	OutPicturesColors.AddDefaulted(OutRGBADatas.size());

	for (int32 WebpIndex = 0; WebpIndex < OutRGBADatas.size(); WebpIndex++)
	{
		TArray<FColor>& OneWebpColor = OutPicturesColors[WebpIndex];

		OneWebpColor.AddDefaulted(OutWebpWidth * OutWebpHeight);


		FMemory::Memcpy(OneWebpColor.GetData(), OutRGBADatas[WebpIndex], OutWebpWidth * OutWebpHeight * 4);
		OutWebpTimestepMillisecond.Add(OutTimestamps_ms[WebpIndex]);

		free((void*)OutRGBADatas[WebpIndex]);


	}


	return bLoadWebp;
}

bool FXGSampleWebPCore::CheckWebpPicturePath(const FString& InGeneratedWebpPicturesPath)
{
	FString Extension = FPaths::GetExtension(InGeneratedWebpPicturesPath);
	FString Directory = FPaths::GetPath(InGeneratedWebpPicturesPath);

	if (Extension.Equals(TEXT("webp")) && FPaths::DirectoryExists(Directory))
	{
		return true;
	}

	UE_LOG(LogXGSampleWebP, Warning, TEXT("FXGWebpCore::CheckWebpPicturePath Wrong Path"));
	return false;
}

bool FXGSampleWebPCore::GetViewportSize(UObject* WorldContextObject, FVector2D& OutViewportSize)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		return false;

	}
	if (UGameViewportClient* InViewportClient = WorldContextObject->GetWorld()->GetGameViewport())
	{
		if (FSceneViewport* InViewport = InViewportClient->GetGameViewport())
		{
			OutViewportSize = FVector2D::ZeroVector;
			InViewportClient->GetViewportSize(OutViewportSize);

			return true;
		}
	}




	return false;
}

bool FXGSampleWebPCore::CheckInWebpPictureSize(UObject* WorldContextObject, TSharedPtr<FXGSampleWebpPictureInformation> InWebpPictureInformation)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		return false;

	}
	FVector2D ViewportSize;
	if (GetViewportSize(WorldContextObject, ViewportSize))
	{

		bool	bX0 = InWebpPictureInformation->X0 >= 0 && InWebpPictureInformation->X0 < InWebpPictureInformation->X1;
		bool	bX1 = InWebpPictureInformation->X1 <= ViewportSize.X-1;

		bool	bY0 = InWebpPictureInformation->Y0 >= 0 && InWebpPictureInformation->Y0 < InWebpPictureInformation->Y1;
		bool	bY1 = InWebpPictureInformation->Y1 <= ViewportSize.Y-1;

		if (bX0 && bX1 && bY0 && bY1)
		{
			return true;
		}


	}

	UE_LOG(LogXGSampleWebP, Warning, TEXT("FXGSampleWebPCore::CheckInWebpPictureSize Wrong WebpPictureSize"));
	return false;


}

