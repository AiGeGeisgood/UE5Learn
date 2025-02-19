#include "XGSimpleRunnable.h"
#include "XGThreadSubsystem.h"
#include "HAL/ThreadManager.h"

bool FXGSimpleRunnable::Init()
{
	//这是新线程

	bRunning = true;

	UXGThreadSubsystem::PrintWarning(TEXT("XGSimpleRunnable在初始化"));
	PrintThreadInfo();
	return true;
}

uint32 FXGSimpleRunnable::Run()
{
	//这是新线程

	UXGThreadSubsystem::PrintWarning(TEXT("XGSimpleRunnable准备进入工作"));
	PrintThreadInfo();
	while (bRunning)
	{
		FPlatformProcess::Sleep(0.04);
	
		FScopeLock Lock(&CriticalSection);
	
		if (!bRunning)
		{
			break;
		}
	
		//不能这样打印日志
		//UE_LOG(LogTemp,Warning,TEXT("新线程在工作"));
	
		UXGThreadSubsystem::PrintWarning(TEXT("XGSimpleRunnable在工作"));
	
	}



	return 0;
}

void FXGSimpleRunnable::Exit()
{
	//这是新线程

	UXGThreadSubsystem::PrintWarning(TEXT("XGSimpleRunnable在退出"));
	PrintThreadInfo();
}

void FXGSimpleRunnable::Stop()
{
	//这不是新线程,是由外部调用方的线程

	UXGThreadSubsystem::PrintWarning(TEXT("XGSimpleRunnable被调用停止"));
	PrintThreadInfo();

	FScopeLock Lock(&CriticalSection);
	bRunning = false;



}

FString FXGSimpleRunnable::GetThreadName()
{
	return XGSimpleRunnableThreadName;
}

void FXGSimpleRunnable::PrintThreadInfo()
{
	uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
	FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
	FString XGInfo = FString::Printf(TEXT("--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
	UXGThreadSubsystem::PrintWarning(XGInfo);

}
