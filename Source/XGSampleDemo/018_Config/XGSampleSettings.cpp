#include "XGSampleSettings.h"

UXGSampleSettings::UXGSampleSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

UXGSampleSettings::~UXGSampleSettings()
{

}

FName UXGSampleSettings::GetContainerName() const
{
	return TEXT("Project");
}

FName UXGSampleSettings::GetCategoryName() const
{
	return TEXT("XG");
}

FName UXGSampleSettings::GetSectionName() const
{
	return TEXT("XGSampleSettings");
}

UXGSampleSettings* UXGSampleSettings::GetXGXunFeiCoreSettings()
{

	UXGSampleSettings* Settings = GetMutableDefault<UXGSampleSettings>();

	return Settings;
}
