#include "ThreadProxy.h"

#include "MyGameMode.h"
#include "Net/Core/Misc/NetConditionGroupManager.h"

FXGThread::FXGThread()
	: Thread(nullptr),
	  StartUpEvent(FPlatformProcess::GetSynchEventFromPool())
{
}

FXGThread::~FXGThread()
{
	if (Thread != nullptr)
	{
		delete Thread;
		Thread = nullptr;
	}

	FPlatformProcess::ReturnSynchEventToPool(StartUpEvent);
}

bool FXGThread::Init()
{
	if (!FPlatformProcess::SupportsMultithreading())
	{
		Stop();
	}
	return StopTaskCounter.GetValue() == 0;
}

uint32 FXGThread::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
		FPlatformProcess::Sleep(0.5f);
		UE_LOG(LogTemp, Warning, TEXT("FXGThread::Run()"));
		MyStruct* ptr = AMyGameMode::GetStr();
		int a = ptr->GetA();
		++a;
		ptr->SetA(a);
		UE_LOG(LogTemp, Warning, TEXT("a = %d"), a);
	}

	return 0;
}

void FXGThread::Stop()
{
	StopTaskCounter.Increment();
}

void FXGThread::Exit()
{
	StartUpEvent->Trigger();
}

void FXGThread::CreateSafeThread()
{
	Thread = FRunnableThread::Create(this,TEXT("XGThread"), 0, TPri_BelowNormal);
}

void FXGThread::WaitAndCompleted()
{
	Stop();
	StartUpEvent->Wait();
	FPlatformProcess::Sleep(0.001f);
}
