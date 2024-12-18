#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "XGSampleSettings.generated.h"

USTRUCT(BlueprintType)
struct FXGSampleDescriber
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XGProjectInfo")
	FString AuthorName = TEXT("XG");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XGProjectInfo")
	bool bExperiment = false;


};


/**
 * UXGSampleSettings
 * My Project Information
 */
UCLASS(Config = XGSampleSettings, defaultconfig)
class XGSAMPLEDEMO_API UXGSampleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UXGSampleSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGSampleSettings();

public:

	virtual FName GetContainerName() const;

	virtual FName GetCategoryName() const;

	virtual FName GetSectionName() const;


public:

	static UXGSampleSettings* GetXGXunFeiCoreSettings();


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XG")
	FString ProjectVersion =TEXT("1.0.0");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XG")
	FXGSampleDescriber  SampleDescriber;

	UPROPERTY(Config,EditAnywhere, BlueprintReadWrite, Category = "XG")
	FString ProjectSimpleName = TEXT("1.0.0");


	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "XG")
	FString XGAppID = TEXT("None");

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "XG")
	FString XGAppKey = TEXT("None");
};