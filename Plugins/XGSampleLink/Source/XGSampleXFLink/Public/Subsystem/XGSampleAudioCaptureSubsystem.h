#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioCapture.h"

#include "XGSampleAudioCaptureSubsystem.generated.h"

UCLASS()
class XGSAMPLEXFLINK_API UXGSampleAudioCaptureSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	friend class UXGSampleXFLinkBPLibrary;
	friend class UXGSampleSTTSubsystem;
	friend class FXGSampleConsumeVoiceRunnable;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

protected:

	static UXGSampleAudioCaptureSubsystem* Get();

	void StartCapturingAudio();

	void StopCapturingAudio();

	bool IsCapturingAudio();

	bool GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo);

	float GetVolumeSize();

	void  SetVolumeSize(float InVolumeSize);


	static void AppendVoiceData(const float* InAudio, int32 NumSamples);

	static bool GetVoiceData(TArray<float>& OutVoiceData);

	static void ClearVoiceData();

	static void OnAudioGenerate(const float* InAudio, int32 NumSamples);

	static TArray<float> LinearResample(TArray<float>& InData, int32 OutDataSize);




protected:

	static UXGSampleAudioCaptureSubsystem* XunFeiAudioCaptureSubsystemPtr;
	
	static int32 AudioCaptureSampleRate;

	static int32 OnceSendFloatDataNum;



	UPROPERTY()
	UAudioCapture* XGAudioCapture;

	FAudioGeneratorHandle AudioGeneratorHandle;

	static FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

	static FCriticalSection VoiceDataSection;

	static TArray<float> NewAllVoiceData;

	float VolumeSize = 0.f;


};