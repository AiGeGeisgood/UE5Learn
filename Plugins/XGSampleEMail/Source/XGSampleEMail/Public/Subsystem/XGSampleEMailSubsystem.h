#pragma once

#include "CoreMinimal.h"
#include "XGSampleEMailType.h"
#include "XGSampleEmailAsyncAction.h"



class FXGSampleEmailSubsystem
{

public:

	FXGSampleEmailSubsystem();

	~FXGSampleEmailSubsystem();


public:

	static FXGSampleEmailSubsystem* GetXGEmailSubsystem();

	static void RealeaseXGEmailSubsystem();


	FGuid CreateEmailAsyncAction(FXGSampleEmailInfo InEmailInfo,
		FXGSampleEmailDelegate InEmailDelegate);

	TArray<FGuid> GetAllEmailAsyncActionIDs();

	bool QueryPointedEmailAsyncActionStatus(FGuid InAsyncID, EXGSampleEMailAsyncActionStatus& OutActionStatus);

	bool RemovePointedEmailAsyncAction(FGuid InAsyncID);

	void RealeaseAllEmailAsyncActions();




private:


	static FXGSampleEmailSubsystem* XGEmailSubsystem;

	TMap<FGuid, TSharedPtr<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe>> XGEmailAsyncActions;


};

