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
	XGSimpleRunnable = MakeShared<FXGSimpleRunnable>(TEXT("XGThread000"));

	XGSimpleThreadPtr = FRunnableThread::Create(XGSimpleRunnable.Get(), *XGSimpleRunnable->GetThreadName());
}

void UXGThreadSubsystem::ReleaseXGSimpleThread()
{
	if (XGSimpleRunnable.IsValid())
	{
		XGSimpleRunnable->Stop();
	}

	if (XGSimpleThreadPtr)
	{
		XGSimpleThreadPtr->WaitForCompletion();
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
			FPlatformProcess::Sleep(2);

			SetMyNum(Max);
		});
	});
}

void UXGThreadSubsystem::BlockThreadPool()
{
	for (size_t i = 0; i < 100; i++)
	{
		AsyncTask(ENamedThreads::AnyThread, []()
		{
			FPlatformProcess::Sleep(2);
		});
	}
}

void UXGThreadSubsystem::BlockThreadPool2()
{
	int64 MyFirstTick = FDateTime::Now().GetTicks();

	// for (int32 i = 0; i < 10; i++)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), i);
	// 	FPlatformProcess::Sleep(2);
	// }

	//ParallelFor利用其他线程来进行for循环，循环时间骤减
	ParallelFor(100, [](int32 Index)
	{
		UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
		FPlatformProcess::Sleep(2);
	});

	int64 MyLastTick = FDateTime::Now().GetTicks();

	UE_LOG(LogTemp, Warning, TEXT("TickDelta_ParallelFor:%lld"), MyLastTick - MyFirstTick);
}

void UXGThreadSubsystem::BlockThreadPool3()
{
	//一样会卡线程
	AsyncTask(ENamedThreads::AnyThread, []()
	{
		int64 MyFirstTick = FDateTime::Now().GetTicks();

		// for (int32 i = 0; i < 10; i++)
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), i);
		// 	FPlatformProcess::Sleep(0.5);
		// }
		
		ParallelFor(10, [](int32 Index)
		{
			UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
			FPlatformProcess::Sleep(0.5);
		}, EParallelForFlags::Unbalanced);

		int64 MyLastTick = FDateTime::Now().GetTicks();

		UE_LOG(LogTemp, Warning, TEXT("TickDelta_ParallelFor:%lld"), MyLastTick - MyFirstTick);
	});
}

void UXGThreadSubsystem::InitAsyn()
{
	//不会阻塞主线程

	AsyncTask(ENamedThreads::AnyThread, []()
	{
		for (size_t i = 0; i < 100; i++)
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
		for (size_t i = 0; i < 100; i++)
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
		for (size_t i = 0; i < 100; i++)
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

		for (size_t i = 0; i < 100; i++)
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
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
		PrintWarning(TEXT("我是SimpleEvent执行完成了"));
		PrintWarning(XGInfo);
	});

	// check(!SimpleEvent->IsComplete());
	auto b = SimpleEvent->IsComplete();

	// SimpleEvent->Wait();
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(SimpleEvent);


	FGraphEventArray SimpleEventArray;

	for (size_t index = 0; index < 20; index++)
	{
		SimpleEventArray.Add(FFunctionGraphTask::CreateAndDispatchWhenReady([index]()
		{
			FString EventInfo = FString::Printf(TEXT("我是SimpleEvent开始执行--Index:%d"), index);

			PrintWarning(EventInfo);

			FPlatformProcess::Sleep(index / 3);
			uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
			FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


			FString XGInfo = FString::Printf(
				TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
			FString EventInfo1 = FString::Printf(TEXT("我是SimpleEvent完成执行--Index:%d"), index);

			PrintWarning(EventInfo1);
			PrintWarning(XGInfo);
		}));
	}

	FTaskGraphInterface::Get().WaitUntilTasksComplete(MoveTemp(SimpleEventArray));

	for (size_t index = 0; index < 20; index++)
	{
		SimpleEventArray.Add(FFunctionGraphTask::CreateAndDispatchWhenReady([index]()
		{
			FString EventInfo = FString::Printf(TEXT("开始执行--Index:%d"), index);

			PrintWarning(EventInfo);

			FPlatformProcess::Sleep(index / 3);
			uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
			FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


			FString XGInfo = FString::Printf(
				TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
			FString EventInfo1 = FString::Printf(TEXT("完成执行--Index:%d"), index);

			PrintWarning(EventInfo1);
			PrintWarning(XGInfo);
		}));
	}
}

void UXGThreadSubsystem::BatchGraphEvent()
{
	//A->B->C->D
	//A->A1->D

	FGraphEventRef SimpleEventA = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventA开始执行"));

		FPlatformProcess::Sleep(2);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
		PrintWarning(XGInfo + TEXT("我是SimpleEventA执行完成了"));
	});


	FGraphEventRef SimpleEventB = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventB开始执行"));

		FPlatformProcess::Sleep(2);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
		PrintWarning(XGInfo + TEXT("我是SimpleEventB执行完成了"));
	}, TStatId{}, SimpleEventA);


	FGraphEventRef SimpleEventC = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventC开始执行"));

		FPlatformProcess::Sleep(2);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
		PrintWarning(XGInfo + TEXT("我是SimpleEventC执行完成了"));
	}, TStatId{}, SimpleEventB);


	FGraphEventRef SimpleEventA1 = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventA111开始执行"));

		FPlatformProcess::Sleep(1);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
		PrintWarning(XGInfo + TEXT("我是SimpleEventA1111执行完成了"));
	}, TStatId{}, SimpleEventA);


	FGraphEventArray Prerequisite;

	Prerequisite.Add(SimpleEventA1);
	Prerequisite.Add(SimpleEventC);


	FGraphEventRef SimpleEventD = FFunctionGraphTask::CreateAndDispatchWhenReady([]()
	{
		PrintWarning(TEXT("我是SimpleEventD开始执行"));

		FPlatformProcess::Sleep(2);
		uint32 CurrentID = FPlatformTLS::GetCurrentThreadId();
		FString CurrentThread = FThreadManager::Get().GetThreadName(CurrentID);


		FString XGInfo = FString::Printf(
			TEXT("--我是主线程02--CurrentID:[%d]--CurrentThreadName:[%s]"), CurrentID, *CurrentThread);
		PrintWarning(XGInfo + TEXT("我是SimpleEventD执行完成了"));
	}, TStatId{}, &Prerequisite);

	//主线程等待该线程执行完成
	// SimpleEventD->Wait();

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

	ParallelFor(50, [](int32 Index)
	            {
		            UE_LOG(LogTemp, Display, TEXT("this is ParallelFor:%d"), Index);
		            FPlatformProcess::Sleep(0.2);
	            },/* EParallelForFlags::ForceSingleThread |*/ /*EParallelForFlags::BackgroundPriority |*/
	            EParallelForFlags::Unbalanced);

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
