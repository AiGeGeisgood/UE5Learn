#include "XGThreadSubsystem.h"
#include "HAL/ThreadManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Async/TaskGraphInterfaces.h"

bool UXGThreadSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGThreadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UXGThreadSubsystem::Deinitialize()
{
	ReleaseXGSimpleThread();
	Super::Deinitialize();
}

void UXGThreadSubsystem::InitXGSimpleThread()
{
	if (!XGSimpleRunnable.IsValid())
	{
		XGSimpleRunnable = MakeShared<FXGSimpleRunnable>(TEXT("XGThread000"));
	}

	if (!XGSimpleThreadPtr)
	{
		XGSimpleThreadPtr = FRunnableThread::Create(XGSimpleRunnable.Get(), *XGSimpleRunnable->GetThreadName());
	}
}

void UXGThreadSubsystem::ReleaseXGSimpleThread()
{
	if (XGSimpleRunnable.IsValid())
	{
		//主线程调用Stop()后，分支线程会执行Exit()
		XGSimpleRunnable->Stop();
	}

	if (XGSimpleThreadPtr)
	{
		//等待分支线执行Exit()
		XGSimpleThreadPtr->WaitForCompletion();

		delete XGSimpleThreadPtr;

		XGSimpleThreadPtr = nullptr;
	}
}

void UXGThreadSubsystem::InitXGSimpleThreadBool()
{
	XGSimpleRunnableBool = MakeShared<FXGSimpleRunnable>(TEXT("XGThreadBool000"));

	XGSimpleThreadPtrBool = FRunnableThread::Create(XGSimpleRunnableBool.Get(), *XGSimpleRunnableBool->GetThreadName());
}

void UXGThreadSubsystem::ReleaseXGSimpleThreadBool()
{
	if (XGSimpleRunnableBool.IsValid())
	{
		XGSimpleRunnableBool->Stop();
	}

	if (XGSimpleThreadPtrBool)
	{
		XGSimpleThreadPtrBool->WaitForCompletion();
	}
}

void UXGThreadSubsystem::InitAsynTask()
{
	//第N帧数
	AsyncTask(ENamedThreads::GameThread, []()
	{
		//N+1帧数

		//在N+2帧数-A
		PrintWarning(TEXT("我是主线程00"));

		//在N+1帧数
		UE_LOG(LogTemp, Warning, TEXT("我是主线程01"));

		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);

		//在N+2帧数-B
		UXGThreadSubsystem::PrintWarning(XGInfo);
	});
}

void UXGThreadSubsystem::InitAsynTask_ALotOfWork()
{
	int32 CopyNum = MyNum;

	AsyncTask(ENamedThreads::AnyThread, [this, CopyNum]()
	{
		int32 Max = CopyNum;

		for (size_t i = 0; i < 10000; i++)
		{
			Max += i;
		}

		FPlatformProcess::Sleep(10);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是临时线程执行完毕--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);


		AsyncTask(ENamedThreads::GameThread, [this, Max]()
		{
			FPlatformProcess::Sleep(10);

			SetMyNum(Max);
		});
	});
}

void UXGThreadSubsystem::BlockThreadPool()
{
	//开辟的线程少就不阻塞主线，支线运行
	for (size_t i = 0; i < 10; i++)
	{
		AsyncTask(ENamedThreads::AnyThread, [&i]()
		{
			FPlatformProcess::Sleep(10);
			uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
			FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
			FString XGInfo = FString::Printf(
				TEXT("I：[%llu]--CurrentID:[%d]--CurrentThreadName:[%s]"), i, CurrentID, *CurrentThread);
			UXGThreadSubsystem::PrintWarning(XGInfo);
		});
	}
	
}

void UXGThreadSubsystem::BlockThreadPool2()
{
	//一定阻塞主线，只是多线程并行
	ParallelFor(10, [](int32 Index)
	{																												
		UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
		FPlatformProcess::Sleep(10);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
		FPlatformProcess::Sleep(10);
		FString XGInfo = FString::Printf(
			TEXT("I：[%llu]--CurrentID:[%d]--CurrentThreadName:[%s]"), Index, CurrentID, *CurrentThread);
		UXGThreadSubsystem::PrintWarning(XGInfo);
	});
}

void UXGThreadSubsystem::BlockThreadPool3()
{
	//一样会卡线程
	AsyncTask(ENamedThreads::AnyThread, []()
	{
		ParallelFor(10, [](int32 Index)
		{
			UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
			FPlatformProcess::Sleep(2);

			uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
			FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
			FPlatformProcess::Sleep(10);
			FString XGInfo = FString::Printf(
				TEXT("I：[%llu]--CurrentID:[%d]--CurrentThreadName:[%s]"), Index, CurrentID, *CurrentThread);
			UXGThreadSubsystem::PrintWarning(XGInfo);
		});
	});
}

void UXGThreadSubsystem::InitAsyn()
{
	//不会阻塞主线程

	AsyncTask(ENamedThreads::AnyThread, []()
	{
		for (size_t i = 0; i < 10; i++)
		{
			Async(EAsyncExecution::Thread, []()
			{
				FPlatformProcess::Sleep(2);

				uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
				FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
				FString XGInfo = FString::Printf(
					TEXT("--我是临时线程执行完毕--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);

				PrintWarning(XGInfo);
			});
		}
	});
}

void UXGThreadSubsystem::InitAsyn02()
{
	//阻塞主线程
	AsyncTask(ENamedThreads::AnyThread, []()
	{
		for (size_t i = 0; i < 10; i++)
		{
			Async(EAsyncExecution::TaskGraph, []()
			{
				FPlatformProcess::Sleep(2);

				uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
				FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
				FString XGInfo = FString::Printf(
					TEXT("--我是临时线程执行完毕--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);

				PrintWarning(XGInfo);
			});
		}
	});
}

void UXGThreadSubsystem::InitAsyn03()
{
	//不会阻塞主线程
	AsyncTask(ENamedThreads::AnyThread, []()
	{
		for (size_t i = 0; i < 10; i++)
		{
			Async(EAsyncExecution::ThreadPool, []()
			{
				FPlatformProcess::Sleep(2);

				uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
				FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
				FString XGInfo = FString::Printf(
					TEXT("--我是临时线程执行完毕--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);

				PrintWarning(XGInfo);
			});
		}
	});
}

void UXGThreadSubsystem::GetAsynFuture()
{
	AsyncTask(ENamedThreads::AnyThread, []()
	{
		TArray<TFuture<int32>> FutureResults;

		for (size_t i = 0; i < 10; i++)
		{
			FutureResults.AddDefaulted();
			FutureResults.Last() =
				Async(EAsyncExecution::Thread, [i]()-> int32
				{
					int32 SleepTime = i % 5;

					FPlatformProcess::Sleep(SleepTime);

					uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
					FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);
					FString XGInfo = FString::Printf(
						TEXT("--我是临时线程执行完毕--CurrentID:[%d]--CurrentThreadName:[%s]--SleepTime:%d"),
						CurrentID,
						*CurrentThread,
						SleepTime
					);

					PrintWarning(XGInfo);

					return SleepTime;
				});
		}
		//50%
		PrintWarning(TEXT("所有任务构建完毕"));

		for (auto& TmpFuture : FutureResults)
		{
			int32 SleepTime = TmpFuture.Get();

			PrintWarning(FString::FromInt(SleepTime));
		}

		//100% 呼叫主线程执行后续操作
		PrintWarning(TEXT("所有任务执行完毕"));
	});
}

//FFunctionGraphTask
//FGraphEventRef
//FGraphEventArray
void UXGThreadSubsystem::GraphEvent()
{
	FGraphEventRef SimpleEvent = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEvent开始执行"));

		FPlatformProcess::Sleep(3);

		PrintWarning(TEXT("我是SimpleEvent执行完成了"));
	});

	check(!SimpleEvent->IsComplete());

	SimpleEvent->Wait();


	FGraphEventArray SimpleEventArray;

	for (size_t index = 0; index < 20; index++)
	{
		SimpleEventArray.Add(FFunctionGraphTask::CreateAndDispatchWhenReady([index]()
		{
			FString EventInfo = FString::Printf(TEXT("我是SimpleEvent开始执行--Index:%d"), index);

			PrintWarning(EventInfo);

			FPlatformProcess::Sleep(index / 3);

			FString EventInfo1 = FString::Printf(TEXT("我是SimpleEvent完成执行--Index:%d"), index);

			PrintWarning(EventInfo1);
		}));
	}


	FTaskGraphInterface::Get().WaitUntilTasksComplete(MoveTemp(SimpleEventArray));
}

void UXGThreadSubsystem::BatchGraphEvent()
{
	//A->B->C->D
	//A->A1->D

	FGraphEventRef SimpleEventA = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventA开始执行"));

		FPlatformProcess::Sleep(2);

		PrintWarning(TEXT("我是SimpleEventA执行完成了"));
	});


	FGraphEventRef SimpleEventB = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventB开始执行"));

		FPlatformProcess::Sleep(2);

		PrintWarning(TEXT("我是SimpleEventB执行完成了"));
	}, TStatId{}, SimpleEventA);


	FGraphEventRef SimpleEventC = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventC开始执行"));

		FPlatformProcess::Sleep(2);

		PrintWarning(TEXT("我是SimpleEventC执行完成了"));
	}, TStatId{}, SimpleEventB);


	FGraphEventRef SimpleEventA1 = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventA111开始执行"));

		FPlatformProcess::Sleep(1);

		PrintWarning(TEXT("我是SimpleEventA1111执行完成了"));
	}, TStatId{}, SimpleEventA);


	FGraphEventArray Prerequisite;

	Prerequisite.Add(SimpleEventA1);
	Prerequisite.Add(SimpleEventC);


	FGraphEventRef SimpleEventD = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventD开始执行"));

		FPlatformProcess::Sleep(2);

		PrintWarning(TEXT("我是SimpleEventD执行完成了"));
	}, TStatId{}, &Prerequisite);

	SimpleEventD->Wait();

	PrintWarning(TEXT("所有任务均已完成"));
}

void UXGThreadSubsystem::InitParallelFor()
{
	int64 MyLastTick = FDateTime::Now().GetTicks();

	for (size_t i = 0; i < 10; i++)
	{
		FPlatformProcess::Sleep(0.2);
	}

	int64 MyCurrentTick = FDateTime::Now().GetTicks();

	int64 TickDelta = MyCurrentTick - MyLastTick;

	UE_LOG(LogTemp, Warning, TEXT("TickDelta:%d"), TickDelta);


	MyLastTick = FDateTime::Now().GetTicks();

	ParallelFor(10, [](int32 Index)
	{
		UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
		FPlatformProcess::Sleep(0.2);
	}, EParallelForFlags::ForceSingleThread | EParallelForFlags::BackgroundPriority | EParallelForFlags::Unbalanced);

	MyCurrentTick = FDateTime::Now().GetTicks();

	TickDelta = MyCurrentTick - MyLastTick;

	UE_LOG(LogTemp, Warning, TEXT("TickDelta_ParallelFor:%d"), TickDelta);
}

void UXGThreadSubsystem::InitParallelFor_Lock()
{
	int64 MyLastTick = FDateTime::Now().GetTicks();
	int32 MaxNum = 0;
	for (size_t i = 0; i < 10; i++)
	{
		MaxNum += i;

		FPlatformProcess::Sleep(i % 3);
	}

	int64 MyCurrentTick = FDateTime::Now().GetTicks();

	int64 TickDelta = MyCurrentTick - MyLastTick;

	UE_LOG(LogTemp, Warning, TEXT("TickDelta:%d--MaxNum:[%d]"), TickDelta, MaxNum);


	MyLastTick = FDateTime::Now().GetTicks();

	FCriticalSection NumCriticalSection;
	MaxNum = 0;

	ParallelFor(10, [&MaxNum, &NumCriticalSection](int32 Index)
	{
		FPlatformProcess::Sleep(Index % 3);

		FScopeLock Lock(&NumCriticalSection);
		MaxNum += Index;

		UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
	}, EParallelForFlags::BackgroundPriority | EParallelForFlags::Unbalanced);

	MyCurrentTick = FDateTime::Now().GetTicks();

	TickDelta = MyCurrentTick - MyLastTick;

	UE_LOG(LogTemp, Warning, TEXT("TickDelta:%d--MaxNum:[%d]"), TickDelta, MaxNum);
}

int32 UXGThreadSubsystem::GetMyNum()
{
	return MyNum;
}

void UXGThreadSubsystem::SetMyNum(int32 InInt)
{
	if (!IsInGameThread())
	{
		AsyncTask(ENamedThreads::GameThread, [this, InInt]() { SetMyNum(InInt); });
	}

	MyNum = InInt;
}

void UXGThreadSubsystem::PrintWarning(FString InStr)
{
	AsyncTask(ENamedThreads::GameThread, [InStr]()
	{
		UE_LOG(LogTemp, Warning, TEXT("ThreadLog:[%s]"), *InStr)
	});
}
