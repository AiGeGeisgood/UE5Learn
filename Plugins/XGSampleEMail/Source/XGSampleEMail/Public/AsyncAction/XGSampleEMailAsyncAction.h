#pragma once

#include "CoreMinimal.h"

#include "XGSampleEMailRunnable.h"

#include "XGSampleEMailType.h"


class FSocket;

class FXGSampleEmailAsyncAction : public TSharedFromThis<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe>
{

	friend class FXGSampleEmailSubsystem;

public:

	FXGSampleEmailAsyncAction();

	virtual ~FXGSampleEmailAsyncAction();

	static TSharedPtr<FXGSampleEmailAsyncAction> CreateEmailAsyncAction(FXGSampleEmailInfo InEmailInfo, FXGSampleEmailDelegate InEmailDelegate);

	void StopEmailAsyncAction();

	FGuid GetAsyncActionID();

	EXGSampleEMailAsyncActionStatus GetAsyncActionStatus();

	void Activate();

protected:

	void Activate_Internal();

	void CallEmailDelegate(FGuid InAsyncID, bool bResult, FString InMessage);

	void CreateEmailRunnable();

	void CloseEmailRunnable();

	void NotifyEmailSubsystemToRemoveSelf();

	void StageClose(const FString& InStageStr, const FString& InMessage, bool bSuccess = false);


protected:

	void OnInit();

	void OnInitError(const FString& InMessage);

	void OnMessage(const FString& InMessage);

	void OnMessageSent(bool InSentResult, const FString& InMessage);

	void OnClosed(const FString& InMessage);

protected:

	FString GetEmailServerIP();

	void Send(FString InMessage);



private:

	FGuid AsyncID = FGuid();

	EXGSampleEMailAsyncActionStatus XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::None;

	FXGSampleEmailInfo EmailInfo;

	FXGSampleEmailDelegate EmailDelegate;

	FString StageMessage = TEXT("");
	
	TSharedPtr<FXGSampleEmailRunnable> XGEmailRunnable;

	FRunnableThread* XGEmailRunnableThread;

};