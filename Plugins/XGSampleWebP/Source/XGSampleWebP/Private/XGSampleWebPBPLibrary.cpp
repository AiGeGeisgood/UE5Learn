#include "XGSampleWebPBPLibrary.h"

#include "decode.h"
#include "demux.h"
#include "mux.h"
#include "encode.h"

#include "Kismet/GameplayStatics.h"

#include "XGSampleWebPCore.h"
#include "LogXGSampleWebP.h"
#include "XGSampleWebPMultiShotSubsystem.h"
#include "XGSampleWebpShowMultiSubsystem.h"
#include "Engine/GameInstance.h"


bool UXGSampleWebPBPLibrary::bAuth = false;

void UXGSampleWebPBPLibrary::SetXGWebpAuth()
{

	bAuth = true;
	UE_LOG(LogXGSampleWebP, Display, TEXT("XGSample 模拟授权成功"));
}

bool UXGSampleWebPBPLibrary::GetXGSampleWebpVersion(FString& OutVersionInfo)
{

	if (!bAuth)
	{

		UE_LOG(LogXGSampleWebP, Error, TEXT("该插件未授权,无法使用,请联系作者"));

		return false;
	}

	int32 DecoderVersion = WebPGetDecoderVersion();
	int32 DemuxVersion = WebPGetDemuxVersion();
	int32 MuxVersion = WebPGetMuxVersion();
	int32 EncoderVersion = WebPGetEncoderVersion();

	OutVersionInfo = FString::Printf(TEXT("DecoderVersion:[%d],DemuxVersion:[%d],MuxVersion:[%d],EncoderVersion:[%d]"),
		DecoderVersion, DemuxVersion, MuxVersion, EncoderVersion);

	UE_LOG(LogXGSampleWebP, Display, TEXT("VersionInfo:[%s]"), *OutVersionInfo);

	return true;



}

void UXGSampleWebPBPLibrary::BeginRecord(UObject* WorldContextObject,
	FString InGeneratedWebpPicturesPath,
	FXGSampleWebpPictureInformation InWebpPictureInformation,
	bool& bBegin)
{
	bBegin = false;

	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);

	UXGSampleWebPMultiShotSubsystem* XGWebpSubSystem = GameInstance->GetSubsystem<UXGSampleWebPMultiShotSubsystem>();

	TSharedPtr<FXGSampleWebpPictureInformation> XGWebpPictureInformationPtr = MakeShareable(new FXGSampleWebpPictureInformation(InWebpPictureInformation));

	bBegin = XGWebpSubSystem->BeginRecord(InGeneratedWebpPicturesPath, XGWebpPictureInformationPtr);


}

void UXGSampleWebPBPLibrary::BeginRecordFullViewport(UObject* WorldContextObject, FString InGeneratedWebpPicturesPath, bool& bBegin)
{
	bBegin = false;

	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGSampleWebPMultiShotSubsystem* XGWebpSubSystem = GameInstance->GetSubsystem<UXGSampleWebPMultiShotSubsystem>();

	FVector2D ViewportSize;

	if (FXGSampleWebPCore::GetViewportSize(WorldContextObject, ViewportSize))
	{

		TSharedPtr<FXGSampleWebpPictureInformation> WebpPictureInformation = MakeShareable(new FXGSampleWebpPictureInformation(0, 0, ViewportSize.X - 1, ViewportSize.Y - 1));

		bBegin = XGWebpSubSystem->BeginRecord(InGeneratedWebpPicturesPath, WebpPictureInformation);
	}







}

void UXGSampleWebPBPLibrary::EndRecord(UObject* WorldContextObject, FXGWebpFinishGenerateMultiWebp InFinishWebpBPDegelete)
{

	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGSampleWebPMultiShotSubsystem* XGWebpSubSystem = GameInstance->GetSubsystem<UXGSampleWebPMultiShotSubsystem>();

	XGWebpSubSystem->EndRecord(InFinishWebpBPDegelete);


}

void UXGSampleWebPBPLibrary::LoadWebp(UObject* WorldContextObject, FXGWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath)
{
	if (!WorldContextObject)
	{
		InLoadAndShowWebpDegelete.ExecuteIfBound(false,nullptr,-1,-1);
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		InLoadAndShowWebpDegelete.ExecuteIfBound(false, nullptr, -1, -1);
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGSampleWebpShowMultiSubsystem* XGWebpShowSubSystem = GameInstance->GetSubsystem<UXGSampleWebpShowMultiSubsystem>();

	XGWebpShowSubSystem->LoadWebp(InLoadAndShowWebpDegelete, InWebpFilePath);
}

void UXGSampleWebPBPLibrary::ReleaseLoadedWebp(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGSampleWebpShowMultiSubsystem* XGWebpShowSubSystem = GameInstance->GetSubsystem<UXGSampleWebpShowMultiSubsystem>();

	XGWebpShowSubSystem->ReleaseLoadedWebp();

}

