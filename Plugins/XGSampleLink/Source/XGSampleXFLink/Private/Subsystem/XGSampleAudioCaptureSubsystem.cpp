#include "XGSampleAudioCaptureSubsystem.h"

#include "Generators/AudioGenerator.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameInstance.h"
#include "LogXGXF.h"



FCriticalSection UXGSampleAudioCaptureSubsystem::VoiceDataSection;
TArray<float>  UXGSampleAudioCaptureSubsystem::NewAllVoiceData = {};
FAudioCaptureDeviceInfo  UXGSampleAudioCaptureSubsystem::AudioCaptureDeviceInfo;

int32 UXGSampleAudioCaptureSubsystem::AudioCaptureSampleRate = 16000;
int32 UXGSampleAudioCaptureSubsystem::OnceSendFloatDataNum = 640;

UXGSampleAudioCaptureSubsystem* UXGSampleAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr = nullptr;
bool UXGSampleAudioCaptureSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGSampleAudioCaptureSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	XunFeiAudioCaptureSubsystemPtr = this;

	Super::Initialize(Collection);
}

void UXGSampleAudioCaptureSubsystem::Deinitialize()
{
	StopCapturingAudio();
	XunFeiAudioCaptureSubsystemPtr =nullptr;
	Super::Deinitialize();
}

UXGSampleAudioCaptureSubsystem* UXGSampleAudioCaptureSubsystem::Get()
{
	return XunFeiAudioCaptureSubsystemPtr;
}

void UXGSampleAudioCaptureSubsystem::StartCapturingAudio()
{
	if (XGAudioCapture)
	{
		StopCapturingAudio();
	}

	XGAudioCapture = UAudioCaptureFunctionLibrary::CreateAudioCapture();

	if (XGAudioCapture)
	{
		ClearVoiceData();

		GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);

		AudioGeneratorHandle = XGAudioCapture->AddGeneratorDelegate(&UXGSampleAudioCaptureSubsystem::OnAudioGenerate);

		XGAudioCapture->StartCapturingAudio();
	}
	else
	{
		check(false);
	}

}

void UXGSampleAudioCaptureSubsystem::StopCapturingAudio()
{
	if (XGAudioCapture)
	{
		XGAudioCapture->StopCapturingAudio();
		XGAudioCapture->RemoveGeneratorDelegate(AudioGeneratorHandle);
		XGAudioCapture = nullptr;

	}

	ClearVoiceData();

}

bool UXGSampleAudioCaptureSubsystem::IsCapturingAudio()
{
	return XGAudioCapture->IsCapturingAudio();
}

bool UXGSampleAudioCaptureSubsystem::GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo)
{
	if (XGAudioCapture)
	{
		return XGAudioCapture->GetAudioCaptureDeviceInfo(OutInfo);
	}

	return false;
}

float UXGSampleAudioCaptureSubsystem::GetVolumeSize()
{
	return VolumeSize;
}

void UXGSampleAudioCaptureSubsystem::SetVolumeSize(float InVolumeSize)
{

	VolumeSize= InVolumeSize;
}

void UXGSampleAudioCaptureSubsystem::AppendVoiceData(const float* InAudio, int32 NumSamples)
{
	FScopeLock Lock(&VoiceDataSection);

	NewAllVoiceData.Append(InAudio, NumSamples);
}


//取数据不一定成功
bool UXGSampleAudioCaptureSubsystem::GetVoiceData(TArray<float>& OutVoiceData)
{
	FScopeLock Lock(&VoiceDataSection);
	OutVoiceData.Empty();

	//重采样
	if (AudioCaptureDeviceInfo.SampleRate == 48000 && (AudioCaptureDeviceInfo.NumInputChannels == 2 || AudioCaptureDeviceInfo.NumInputChannels == 1)
		|| AudioCaptureDeviceInfo.SampleRate == 16000 && (AudioCaptureDeviceInfo.NumInputChannels == 2 || AudioCaptureDeviceInfo.NumInputChannels == 1))
	{

		int32 AudioFix = AudioCaptureDeviceInfo.NumInputChannels * AudioCaptureDeviceInfo.SampleRate / AudioCaptureSampleRate;

		int32 HanldeAudioFloatNum = OnceSendFloatDataNum * AudioFix;

		if (NewAllVoiceData.Num() > HanldeAudioFloatNum)
		{

			for (int32 Index = 0; Index < OnceSendFloatDataNum; Index++)
			{
				OutVoiceData.Add(NewAllVoiceData[Index * AudioFix]);
			}

			NewAllVoiceData.RemoveAt(0, HanldeAudioFloatNum);

			return true;
		}

	}
	else if (AudioCaptureDeviceInfo.SampleRate == 44100 && AudioCaptureDeviceInfo.NumInputChannels == 2
		|| AudioCaptureDeviceInfo.SampleRate == 44100 && AudioCaptureDeviceInfo.NumInputChannels == 1)
	{
		//每40ms要处理多少个原始44100的采样点数据
		//对于这里单通道 1764个采样成640个
		//25=1000ms/40ms
		int32 HanldeAudioFloatNum = AudioCaptureDeviceInfo.SampleRate / 25 * AudioCaptureDeviceInfo.NumInputChannels;

		if (NewAllVoiceData.Num() > HanldeAudioFloatNum)
		{
			TArray<float> OriginData;
			int32 StepSize = AudioCaptureDeviceInfo.NumInputChannels;

			for (int32 Index = 0; Index < HanldeAudioFloatNum;)
			{
				Index += StepSize;
				OriginData.Add(NewAllVoiceData[Index]);

			}


			OutVoiceData = LinearResample(OriginData, OnceSendFloatDataNum);

			NewAllVoiceData.RemoveAt(0, HanldeAudioFloatNum);

			return true;

		}

	}
	else
	{
		check(false);
	}




	return false;
}

void UXGSampleAudioCaptureSubsystem::ClearVoiceData()
{
	FScopeLock Lock(&VoiceDataSection);

	NewAllVoiceData.Empty();

	if (UXGSampleAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr)
	{
		UXGSampleAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr->SetVolumeSize(0.f);
	}

}
//不稳定流送
void UXGSampleAudioCaptureSubsystem::OnAudioGenerate(const float* InAudio, int32 NumSamples)
{
	//Degbu的代码

	//FAudioCaptureDeviceInfo OutAudioCaptureDeviceInfo = AudioCaptureDeviceInfo;

	//AsyncTask(ENamedThreads::GameThread, [OutAudioCaptureDeviceInfo, NumSamples]() {

	//	UE_LOG(LogXGXFSTT, Warning, TEXT("--NumCaputure:%d--AudioName:[%s]--AudioChannel:[%d]--AudioRate:[%d]"), NumSamples
	//		, *OutAudioCaptureDeviceInfo.DeviceName.ToString(), OutAudioCaptureDeviceInfo.NumInputChannels, OutAudioCaptureDeviceInfo.SampleRate);
	//	});

	//音频线程上执行



	if (NumSamples <= 0)
	{
		return;
	}


	//TODO 计算音频的声音大小

	float Volume = 0.0;

	for (int32 VolumeCalculationIndex = 0; VolumeCalculationIndex < NumSamples; VolumeCalculationIndex++)
	{

		Volume += InAudio[VolumeCalculationIndex] * InAudio[VolumeCalculationIndex];
	}

	Volume = FMath::Sqrt(Volume / NumSamples);

	AsyncTask(ENamedThreads::GameThread, [Volume]() {

		if (UXGSampleAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr)
		{
			UXGSampleAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr->SetVolumeSize(Volume);
		}

		});





	UXGSampleAudioCaptureSubsystem::AppendVoiceData(InAudio, NumSamples);





}

TArray<float> UXGSampleAudioCaptureSubsystem::LinearResample(TArray<float>& InData, int32 OutDataSize)
{
	TArray<float> OutData;

	double ResampleRatio = (float)(InData.Num() - 1) / (float)(OutDataSize - 1);

	for (int32 i = 0; i < OutDataSize; ++i) {
		float Index = i * ResampleRatio;

		int32 LowerIndex = (int32)Index;
		int32 UpperIndex = FMath::Min((int32)Index + 1, InData.Num() - 1);
		float alpha = Index - LowerIndex;

		float interpolatedValue = (1.0 - alpha) * InData[LowerIndex] + alpha * InData[UpperIndex];
		OutData.Add(interpolatedValue);
	}

	return OutData;


}
