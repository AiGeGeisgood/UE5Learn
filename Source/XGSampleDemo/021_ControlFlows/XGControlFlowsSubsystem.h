#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ControlFlowNode.h"
#include "XGControlFlowsSubsystem.generated.h"

//进度更新
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGControlFlowsInitProgress, FGuid, InitAsyncID, float, ProgressValue);

//初始化结果
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FXGControlFlowsInitResult, FGuid, InitAsyncID, bool, bResult, FString, Message);

//A的管理任务完成的回调
DECLARE_MULTICAST_DELEGATE_TwoParams(FXGControlFlowsManageTaskResponseResult, bool, FString)

#define XGCONCTROLRESULT 1

//A1 A2 A3.....
enum class EXGControlFlowsTaskStatus : uint8
{
	None,
	Succeed,
	Failed,
	Max
};

//A
enum class EXGControlFlowsMangeTaskStatus : uint8
{
	None,
	Succeed,
	Failed,
	Processing,
	Max
};



struct FXGControlFlowsTask :public TSharedFromThis<FXGControlFlowsTask>
{
	FXGControlFlowsTask() :
		TaskID(FGuid::NewGuid()),
		BelongToManageTaskID(FGuid()),
		TaskStatus(EXGControlFlowsTaskStatus::None),
		TaskMessage(TEXT("None"))
	{

	}

	virtual ~FXGControlFlowsTask() {}



	FGuid TaskID;
	FGuid BelongToManageTaskID;
	EXGControlFlowsTaskStatus TaskStatus;
	FString TaskMessage;







};

struct FXGControlFlowsManageTask : public TSharedFromThis<FXGControlFlowsManageTask>
{
	FXGControlFlowsManageTask() :
		ManageTaskID(FGuid::NewGuid()),
		MangeTaskStatus(EXGControlFlowsMangeTaskStatus::None),
		ManagTaskMessage(TEXT("None"))
	{

	}

	FGuid ManageTaskID;
	EXGControlFlowsMangeTaskStatus MangeTaskStatus;
	FString ManagTaskMessage;

	//>=1
	int32 TaskNum;
	TArray<TSharedPtr<FXGControlFlowsTask>> Tasks;


	FXGControlFlowsManageTaskResponseResult ManageTaskResponse;

	EXGControlFlowsMangeTaskStatus CheckManageTaskStatus()
	{
		if (TaskNum!= Tasks.Num())
		{
			MangeTaskStatus= EXGControlFlowsMangeTaskStatus::Processing;

			return MangeTaskStatus;
		}



		ManagTaskMessage = TEXT("");
		MangeTaskStatus = EXGControlFlowsMangeTaskStatus::Succeed;


		for (auto& TmpTask : Tasks)
		{
			if (TmpTask->TaskStatus == EXGControlFlowsTaskStatus::Failed)
			{
				ManagTaskMessage += TEXT("Task Failded -Guid:") + TmpTask->TaskID.ToString() + TEXT("--TaskMessage:") + TmpTask->TaskMessage;

				 MangeTaskStatus = EXGControlFlowsMangeTaskStatus::Failed;

			}

			if (TmpTask->TaskStatus == EXGControlFlowsTaskStatus::None)
			{
				MangeTaskStatus = EXGControlFlowsMangeTaskStatus::Processing;

				return EXGControlFlowsMangeTaskStatus::Processing;

			}

			ManagTaskMessage += TEXT("\r\n--SubTaskID:") + TmpTask->TaskID.ToString() + TEXT("--TaskMessage:") + TmpTask->TaskMessage;
		}
		return MangeTaskStatus;
	}


};






UCLASS()
class XGSAMPLEDEMO_API UXGControlFlowsSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;



public:

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;


public:

	UFUNCTION(BlueprintCallable, Category = "XG")
	void InitLevel();



protected:



	void CallInitProgress(FGuid InInitAsyncID, float  ProgressValue);


	void CallInitResult(FGuid InInitAsyncID, bool bResult, FString Message);


	void UpdateTaskStatus(FGuid InTaskID,FGuid InManageTaskID, bool bResult, FString Message);


public:

	UPROPERTY(BlueprintAssignable)
	FXGControlFlowsInitProgress InitProgress;

	UPROPERTY(BlueprintAssignable)
	FXGControlFlowsInitResult InitResult;



protected:

	void	InitLocalAsset(FControlFlowNodeRef SubFlow, double InProgreesValue);

	void	InitNetInfo(FControlFlowNodeRef SubFlow, double InProgreesValue);

	void	InitUserInfo(FControlFlowNodeRef SubFlow, double InProgreesValue);

	void	NotifyMainUI(FControlFlowNodeRef SubFlow, double InProgreesValue);

	void	FinishThisInit(FControlFlowNodeRef SubFlow, double InProgreesValue);

protected:

	bool bIniting = false;
	FGuid InitAsyncID = FGuid();


	TMap<FGuid, TSharedPtr<FXGControlFlowsManageTask>> ManageList;

};
