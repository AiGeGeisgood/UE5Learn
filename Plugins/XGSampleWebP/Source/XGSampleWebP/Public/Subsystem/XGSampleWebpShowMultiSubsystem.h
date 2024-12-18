
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"

#include "XGSampleWebPType.h"

#include "XGSampleWebpShowMultiSubsystem.generated.h"

class 	UTexture2D;
UCLASS()
class  UXGSampleWebpShowMultiSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
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


	void LoadWebp(FXGWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath);

	void ReleaseLoadedWebp();

	void LoadedWebpCallShow(bool bLoaded);

protected:
	
	UPROPERTY()
	UTexture2D* WebpTexture = nullptr;

	EXGSampleWebpLoadAndShowType LoadAndShowStatus = EXGSampleWebpLoadAndShowType::None;

	FXGWebpLoadAndShowWebp XGWebpLoadAndShowWebp;

	int32 WebpShowIndex = -1;
	int32 CurrentMillisecond = -1;

	TArray<int32> WebpTimestepMillisecond;
	TArray<TArray<FColor>> PicturesColors;

	int32 WebpWidth = 0;
	int32 WebpHeight = 0;

};