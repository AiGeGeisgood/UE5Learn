#pragma once

#include "CoreMinimal.h"
#include "XGSimpleRunnable.h"
#include "XGThreadSubsystem.generated.h"

UCLASS()
class XGSAMPLEDEMO_API UXGThreadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


public:


	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitXGSimpleThread();
	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void ReleaseXGSimpleThread();


	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitXGSimpleThreadBool();
	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void ReleaseXGSimpleThreadBool();


	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitAsynTask();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitAsynTask_ALotOfWork();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void BlockThreadPool();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void BlockThreadPool2();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void BlockThreadPool3();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitAsyn();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void GetAsynFuture();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void GraphEvent();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void BatchGraphEvent();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitParallelFor();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void InitParallelFor_Lock();


public:

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	int32 GetMyNum();

	UFUNCTION(BlueprintCallable, Category = "XG|Thread")
	void SetMyNum(int32 InInt);


public:

	static void PrintWarning(FString InStr);


protected:

	TSharedPtr<FXGSimpleRunnable> XGSimpleRunnable;

	FRunnableThread* XGSimpleThreadPtr = nullptr;


	TSharedPtr<FXGSimpleRunnable> XGSimpleRunnableBool;

	FRunnableThread* XGSimpleThreadPtrBool = nullptr;

	int32 MyNum = 0;

};
