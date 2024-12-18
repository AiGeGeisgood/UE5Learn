#pragma once

#include "CoreMinimal.h"


#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"


class FXGSampleConsumeVoiceRunnable :public FRunnable
{


public:

	FXGSampleConsumeVoiceRunnable(FString InThreadName);

	virtual ~FXGSampleConsumeVoiceRunnable();


	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FString ThreadName=TEXT("");

protected:

	FThreadSafeBool bIsRunning = true;
};