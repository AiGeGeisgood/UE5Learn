#include "XGConfigActor.h"
#include "XGSampleSettings.h"
// Sets default values
AXGConfigActor::AXGConfigActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AXGConfigActor::GetMyAppKey(FString& OutMyAppKey)
{
	UXGSampleSettings* XGSampleSettings = UXGSampleSettings::GetXGXunFeiCoreSettings();

	if (XGSampleSettings)
	{
		OutMyAppKey = XGSampleSettings->XGAppKey;
		return true;
	}

	OutMyAppKey = TEXT("None");

	return false;

}

void AXGConfigActor::SetMyAppKey(const FString& InMyAppKey)
{
	UXGSampleSettings* XGSampleSettings = UXGSampleSettings::GetXGXunFeiCoreSettings();

	if (XGSampleSettings)
	{
		XGSampleSettings->XGAppKey = InMyAppKey;

		XGSampleSettings->Modify();
		//这个地方的写法有确认
		FString ConfigPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir() / TEXT("DefaultXGSampleSettings.ini"));

		XGSampleSettings->SaveConfig(CPF_Config, *ConfigPath);

		//XGSampleSettings->PostEditChangeProperty();
	}
}

// Called when the game starts or when spawned
void AXGConfigActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGConfigActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
