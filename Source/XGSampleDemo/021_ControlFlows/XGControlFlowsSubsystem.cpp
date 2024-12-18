#include "XGControlFlowsSubsystem.h"

#include "ControlFlow.h"
#include "ControlFlowManager.h"
#include "Async/Async.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool UXGControlFlowsSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGControlFlowsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


#if XGCONCTROLRESULT

	UE_LOG(LogTemp,Warning,TEXT("正常初始化流程,本次无失败"));


#else 

	UE_LOG(LogTemp, Warning, TEXT("初始化流程,本次有失败"));


#endif

}

void UXGControlFlowsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UXGControlFlowsSubsystem::Tick(float DeltaTime)
{

	TArray<FGuid> FinishManageTaskIDs;

	for (auto& TmpManageTaskPair : ManageList)
	{
		if (TmpManageTaskPair.Value->TaskNum != TmpManageTaskPair.Value->Tasks.Num())
		{
			continue;
		}


		EXGControlFlowsMangeTaskStatus MangeTaskStatus = TmpManageTaskPair.Value->CheckManageTaskStatus();

		if (MangeTaskStatus == EXGControlFlowsMangeTaskStatus::Failed || MangeTaskStatus == EXGControlFlowsMangeTaskStatus::Succeed)
		{
			FinishManageTaskIDs.Add(TmpManageTaskPair.Key);

		}

	}


	for (auto& TmpManageTaskID : FinishManageTaskIDs)
	{
		TSharedPtr<FXGControlFlowsManageTask> XGManageTaskPtr = ManageList[TmpManageTaskID];

		bool bManageTaskResult = XGManageTaskPtr->MangeTaskStatus == EXGControlFlowsMangeTaskStatus::Succeed ? true : false;

		XGManageTaskPtr->ManageTaskResponse.Broadcast(bManageTaskResult, XGManageTaskPtr->ManagTaskMessage);

	}



	for (auto& TmpManageTaskID : FinishManageTaskIDs)
	{

		ManageList.Remove(TmpManageTaskID);
	}





}

bool UXGControlFlowsSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId UXGControlFlowsSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGControlFlowsSubsystem, STATGROUP_Tickables);
}

void UXGControlFlowsSubsystem::InitLevel()
{
	if (bIniting)
	{
		UE_LOG(LogTemp, Warning, TEXT("正在初始化过程中"));
		return;
	}

	InitAsyncID = FGuid::NewGuid();
	bIniting = true;



	uint64 FrameIndex = GFrameCounter;

	UE_LOG(LogTemp, Warning, TEXT("[%s]--InitFlow--FramIndex:[%d]"), *FString(__FUNCTION__), FrameIndex);


	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("XGControlFlowInitLevel"));


	Flow.QueueStep(TEXT("InitLocalAsset"), this, &UXGControlFlowsSubsystem::InitLocalAsset, 0.1);

	Flow.QueueStep(TEXT("InitNetInfo"), this, &UXGControlFlowsSubsystem::InitNetInfo, 0.2);

	Flow.QueueStep(TEXT("InitUserInfo"), this, &UXGControlFlowsSubsystem::InitUserInfo, 0.5);

	Flow.QueueStep(TEXT("NotifyMainUI"), this, &UXGControlFlowsSubsystem::NotifyMainUI, 0.8);

	Flow.QueueStep(TEXT("FinishThisInit"), this, &UXGControlFlowsSubsystem::FinishThisInit, 1.0);

	UE_LOG(LogTemp, Warning, TEXT("[%s]--ExcuteFlow--FramIndex:[%d]"), *FString(__FUNCTION__), FrameIndex);

	Flow.ExecuteFlow();


}

void UXGControlFlowsSubsystem::CallInitProgress(FGuid InInitAsyncID, float ProgressValue)
{
	FXGControlFlowsInitProgress TempDelegate = InitProgress;

	AsyncTask(ENamedThreads::GameThread, [TempDelegate, InInitAsyncID, ProgressValue]() {

		TempDelegate.Broadcast(InInitAsyncID, ProgressValue);

		});

}

void UXGControlFlowsSubsystem::CallInitResult(FGuid InInitAsyncID, bool bResult, FString Message)
{
	FXGControlFlowsInitResult TempDelegate = InitResult;

	AsyncTask(ENamedThreads::GameThread, [TempDelegate, InInitAsyncID, bResult, Message]() {

		TempDelegate.Broadcast(InInitAsyncID, bResult, Message);

		});
}

void UXGControlFlowsSubsystem::UpdateTaskStatus(FGuid InTaskID, FGuid InManageTaskID, bool bResult, FString Message)
{
	TSharedPtr<FXGControlFlowsManageTask>* ManageTaskPtrPtr = ManageList.Find(InManageTaskID);

	if (ManageTaskPtrPtr)
	{
		for (auto& TmpTask : (*ManageTaskPtrPtr)->Tasks)
		{
			if (TmpTask->TaskID == InTaskID)
			{
				TmpTask->TaskStatus = bResult ? EXGControlFlowsTaskStatus::Succeed : EXGControlFlowsTaskStatus::Failed;
				TmpTask->TaskMessage = Message;
			}


		}

	}
	else
	{
		check(false);
	}


}

void UXGControlFlowsSubsystem::InitLocalAsset(FControlFlowNodeRef SubFlow, double InProgreesValue)
{
	uint64 FrameIndex = GFrameCounter;
	UE_LOG(LogTemp, Warning, TEXT("--InitLocalAsset--FramIndex:[%d]"), GFrameCounter);

	AsyncTask(ENamedThreads::AnyThread, [this, SubFlow, InProgreesValue]() {

		FPlatformProcess::Sleep(0.2);

		AsyncTask(ENamedThreads::GameThread, [this, SubFlow, InProgreesValue]() {

			InitProgress.Broadcast(InitAsyncID, InProgreesValue);
			SubFlow->ContinueFlow();


			});

		});







}

void UXGControlFlowsSubsystem::InitNetInfo(FControlFlowNodeRef SubFlow, double InProgreesValue)
{
	//做A任务

	uint64 FrameIndex = GFrameCounter;
	UE_LOG(LogTemp, Warning, TEXT("--InitNetInfo--FramIndex:[%d]"), GFrameCounter);


	//声明一个管理任务A
	TSharedPtr<FXGControlFlowsManageTask>  XGManageTaskPtr = MakeShared<FXGControlFlowsManageTask>();

	//子任务有10个
	XGManageTaskPtr->TaskNum = 10;

	//绑定管理任务完成回调
	XGManageTaskPtr->ManageTaskResponse.AddLambda([SubFlow, InProgreesValue, this](bool bResult, FString Message) {

		//这里必须在主线程
		//A1-A10都完成了
		//回到ABC阶段

		UE_LOG(LogTemp, Warning, TEXT("--InitNetInfo--Result:[%s]--Message:[%s]"), bResult ? TEXT("true") : TEXT("false"), *Message);

		if (bResult)
		{
			CallInitProgress(InitAsyncID, InProgreesValue);
			SubFlow->ContinueFlow();
		}
		else
		{

			CallInitResult(InitAsyncID, bResult, Message);
			SubFlow->CancelFlow();


			bIniting = false;
			InitAsyncID = {};


		}

		});

	//加入到管理任务监听列表
	ManageList.Add(XGManageTaskPtr->ManageTaskID, XGManageTaskPtr);


	//实例化子任务

	for (size_t i = 0; i < 10; i++)
	{
		TSharedPtr<FXGControlFlowsTask> TaskPtr = MakeShared<FXGControlFlowsTask>();

		TaskPtr->BelongToManageTaskID = XGManageTaskPtr->ManageTaskID;

		FGuid ManageTaskID = TaskPtr->BelongToManageTaskID;

		FGuid TaskID = TaskPtr->TaskID;

		XGManageTaskPtr->Tasks.Add(TaskPtr);

		bool bSucceed = true;

		if (i == 6)
		{
			if (XGCONCTROLRESULT)
			{
				bSucceed = true;
			}
			else
			{
				bSucceed = false;
			}

		}

		//声明子任务的执行框架

		AsyncTask(ENamedThreads::AnyThread, [this, bSucceed, TaskID, i, ManageTaskID]() {

			FPlatformProcess::Sleep(i % 3);

			bool bTaskFinishResult = bSucceed;

			FString TaskMessage = FDateTime::Now().ToHttpDate();

			//现在从线程池里面A1任务到主线程ABCD

			AsyncTask(ENamedThreads::GameThread, [this, TaskID, ManageTaskID, bTaskFinishResult, TaskMessage]() {

				//刷新子任务的状态
				UpdateTaskStatus(TaskID, ManageTaskID, bTaskFinishResult, TaskMessage);


				});



			});





	}





}

void UXGControlFlowsSubsystem::InitUserInfo(FControlFlowNodeRef SubFlow, double InProgreesValue)
{

	uint64 FrameIndex = GFrameCounter;
	UE_LOG(LogTemp, Warning, TEXT("--InitUserInfo--FramIndex:[%d]"), GFrameCounter);

	AsyncTask(ENamedThreads::AnyThread, [this, SubFlow, InProgreesValue]() {

		FPlatformProcess::Sleep(0.2);

		FString UserInfoPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("XGControlFlowsUserInfo.txt"));

		FString UserInitAsyncID = InitAsyncID.ToString();
		FString UserInitDataTime = FDateTime::Now().ToHttpDate();

		TArray<FString> MyStringInfo;
		MyStringInfo.Add(UserInitAsyncID);
		MyStringInfo.Add(UserInitDataTime);


		FFileHelper::SaveStringArrayToFile(MyStringInfo, *UserInfoPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		//FFileHelper::LoadFileToStringArray(MyStringInfo, *UserInfoPath);


		AsyncTask(ENamedThreads::GameThread, [this, SubFlow, InProgreesValue]() {

			InitProgress.Broadcast(InitAsyncID, InProgreesValue);
			SubFlow->ContinueFlow();


			});

		});

}

void UXGControlFlowsSubsystem::NotifyMainUI(FControlFlowNodeRef SubFlow, double InProgreesValue)
{
	uint64 FrameIndex = GFrameCounter;
	UE_LOG(LogTemp, Warning, TEXT("--NotifyMainUI--FramIndex:[%d]"), GFrameCounter);

	if (XGCONCTROLRESULT)
	{
		InitProgress.Broadcast(InitAsyncID, InProgreesValue);
		SubFlow->ContinueFlow();
	}
	else
	{


		AsyncTask(ENamedThreads::GameThread, [this]() {
			InitResult.Broadcast(InitAsyncID, false, TEXT("测试失败"));
			});

		//上下是需要根据代码情况取舍的
		SubFlow->CancelFlow();
		bIniting = false;
		InitAsyncID = {};
	}


}

void UXGControlFlowsSubsystem::FinishThisInit(FControlFlowNodeRef SubFlow, double InProgreesValue)
{
	uint64 FrameIndex = GFrameCounter;

	UE_LOG(LogTemp, Warning, TEXT("--FinishThisInit--FramIndex:[%d]"), GFrameCounter);

	InitProgress.Broadcast(InitAsyncID, InProgreesValue);

	InitResult.Broadcast(InitAsyncID, true, TEXT("Init Success"));


	SubFlow->ContinueFlow();

	bIniting = false;
	InitAsyncID = {};

}
