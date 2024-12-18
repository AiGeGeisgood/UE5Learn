
#pragma once

#include "CoreMinimal.h"
#include "XGSampleWebPType.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "XGSampleWebPMultiShotSubsystem.generated.h"

//Webp的结果生成回调
DECLARE_DELEGATE_OneParam(FSampleWebPMultiCallBack, bool)

UCLASS()
class  UXGSampleWebPMultiShotSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend class UXGSampleWebPBPLibrary;


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;



public:

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

protected:

	bool BeginRecord(FString& InGeneratedWebpPicturesPath, TSharedPtr<FXGSampleWebpPictureInformation> InWebpPictureInformation);

	void RecordOneFrame(float DeltaTime);

	void EndRecord(FXGWebpFinishGenerateMultiWebp& InFinshWebpBPDelegate);

	void ResetRecord();

protected:
	UFUNCTION()
	void ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors);


	UFUNCTION()
	void SampleWebPOneMultiCallBackMethod(bool bGenerateWebp);


protected:

	EXGSampleWebpProcessType ProcessType = EXGSampleWebpProcessType::None;
	FString GeneratedWebpPicturesPath =TEXT("");
	TSharedPtr< FXGSampleWebpPictureInformation>  WebpPictureInformation;


	FDelegateHandle ScreenHandle;

	TArray<TArray<FColor>> WebPColor;
	TArray<int32> WebpTimestepMillisecond;

	FXGWebpFinishGenerateMultiWebp FinshWebpBPDelegate;

	FSampleWebPMultiCallBack SampleWebPMultiCallBack;

	FCriticalSection XGWebpMutex;

};