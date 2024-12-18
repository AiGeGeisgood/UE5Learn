// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "XGSampleWebPOneShotSubsystem.generated.h"

//Webp的结果生成回调
DECLARE_DELEGATE_OneParam(FSampleWebPOneShotCallBack, bool)

//反射到蓝图的生成接口回调
DECLARE_DYNAMIC_DELEGATE_OneParam(FSampleWebPOneShotCallBackBP, bool, bFinishGenerate);

UCLASS()
class XGSAMPLEWEBP_API UXGSampleWebPOneShotSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


public:
	UFUNCTION(BlueprintCallable, Category = "XGSampleWebp|Subsystem")
	void BeginSampleWebPOneShot(FSampleWebPOneShotCallBackBP InSampleWebPOneShotCallBackBP);


protected:

	UFUNCTION()
	void SampleWebPOneShotCallBackMethod(bool bGenerateWebp);

	UFUNCTION()
	void ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors);


protected:

	FSampleWebPOneShotCallBack SampleWebPOneShotCallBack;

	FSampleWebPOneShotCallBackBP SampleWebPOneShotCallBackBP;

	FDelegateHandle ScreenHandle;

	bool bWorking =false;

};



