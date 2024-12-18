#pragma once

#include "CoreMinimal.h"


#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"



DECLARE_DELEGATE(FXGEmailSocketInitDelegate);

DECLARE_DELEGATE_OneParam(FXGEmailSocketInitErrorDelegate, const FString&);

DECLARE_DELEGATE_OneParam(FXGEmailSocketReceiveDelegate, const FString&);

DECLARE_DELEGATE_TwoParams(FXGEmailSocketSendDelegate, bool, const FString&);

DECLARE_DELEGATE_OneParam(FXGEmailSocketCloseDelegate, const FString&);


class FSocket;

class FXGSampleEmailRunnable :public FRunnable
{

public:

	FXGSampleEmailRunnable(FString InThreadName, FString InServerIP, int32 InServerPort);

	virtual ~FXGSampleEmailRunnable();


	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

public:

	FXGEmailSocketInitDelegate& OnInit();

	FXGEmailSocketInitErrorDelegate& OnInitError();

	FXGEmailSocketReceiveDelegate& OnMessage();

	FXGEmailSocketSendDelegate& OnMessageSent();

	FXGEmailSocketCloseDelegate& OnClosed();


	void SendMessage(const FString& InMessage);

	FString ThreadName = TEXT("");


protected:

	void CallInitDelegate();

	void CallInitErrorDelegate(const FString& InMessage);

	void CallReceiveDelegate(const FString& InMessage);

	void CallSendDelegate(bool InInitResult, const FString& InMessage);

	void CallCloseDelegate(const FString& InMessage);

protected:

	FString ServerIP = TEXT("");

	int32 ServerPort = 0;

	FXGEmailSocketInitDelegate InitDelegate;

	FXGEmailSocketInitErrorDelegate InitErrorDelegate;

	FXGEmailSocketReceiveDelegate ReceiveDelegate;

	FXGEmailSocketSendDelegate SendDelegate;

	FXGEmailSocketCloseDelegate ClosedDelegate;

	FThreadSafeBool bIsRunning = true;

	FSocket* ConnectSocket;

	TArray<uint8> ReceiveData;

	uint32 ReceiveSize = 1024;

	uint32 SendSize = 1024;

};