#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"

class FXGSimpleRunnableBool :public FRunnable
{

public:

	FXGSimpleRunnableBool(FString InThreadName)
	:XGSimpleRunnableThreadName(InThreadName)
	{

	}

	virtual ~FXGSimpleRunnableBool()
	{


	}

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Exit() override;

	virtual void Stop() override;

	FString GetThreadName();

protected:
	
	void PrintThreadInfo();


protected:

	FString XGSimpleRunnableThreadName =TEXT("None");

	FThreadSafeBool bRunning = true;


};