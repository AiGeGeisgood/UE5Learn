#include "ThreadProxy.h"

#include "Net/Core/Misc/NetConditionGroupManager.h"

XGThread::XGThread()
	: Thread(nullptr),
	  StartUpEvent(FPlatformProcess::GetSynchEventFromPool())
{
}

XGThread::~XGThread()
{
	if (Thread != nullptr)
	{
		delete Thread;
		Thread = nullptr;
	}

	FPlatformProcess::ReturnSynchEventToPool(StartUpEvent);
}

bool XGThread::Init()
{
	if (!FPlatformProcess::SupportsMultithreading())
	{
		Stop();
	}
	return StopTaskCounter.GetValue() == 0;
}

uint32 XGThread::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
	}

	return 0;
}

void XGThread::Stop()
{
	StopTaskCounter.Increment();
}

void XGThread::Exit()
{
	StartUpEvent->Trigger();
}

void XGThread::CreateSafeThread()
{
	Thread = FRunnableThread::Create(this,TEXT("XGThread"), 0, TPri_BelowNormal);
}

void XGThread::WaitAndCompleted()
{
	Stop();
	StartUpEvent->Wait();
	FPlatformProcess::Sleep(0.001f);
}
