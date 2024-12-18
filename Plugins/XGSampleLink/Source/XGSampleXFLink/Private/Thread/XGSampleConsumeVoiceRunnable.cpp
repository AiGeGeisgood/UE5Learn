#include "XGSampleConsumeVoiceRunnable.h"

#include "XGSampleSTTSubsystem.h"
#include"XGSampleAudioCaptureSubsystem.h"

FXGSampleConsumeVoiceRunnable::FXGSampleConsumeVoiceRunnable(FString InThreadName)
{
}

FXGSampleConsumeVoiceRunnable::~FXGSampleConsumeVoiceRunnable()
{
}

bool FXGSampleConsumeVoiceRunnable::Init()
{
	return true;
}

uint32 FXGSampleConsumeVoiceRunnable::Run()
{
	bIsRunning = true;

	while (bIsRunning)
	{
		FPlatformProcess::Sleep(0.01);

		TArray<float> VoiceData;

		bool bRightVoice = UXGSampleAudioCaptureSubsystem::GetVoiceData(VoiceData);

		if (bRightVoice)
		{
			UXGSampleSTTSubsystem::RealTimeSTTSubsystemPtr->SendVoiceData(VoiceData);
		}
	}
	return 0;
}

void FXGSampleConsumeVoiceRunnable::Stop()
{
	bIsRunning = false;

}

void FXGSampleConsumeVoiceRunnable::Exit()
{
	UXGSampleAudioCaptureSubsystem::ClearVoiceData();

}
