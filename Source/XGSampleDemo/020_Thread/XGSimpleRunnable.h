#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class FXGSimpleRunnable :public FRunnable
{

public:

	FXGSimpleRunnable(FString InThreadName)
	:XGSimpleRunnableThreadName(InThreadName)
	{

	}

	virtual ~FXGSimpleRunnable() 
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

	bool bRunning = true;

	FCriticalSection CriticalSection;

};