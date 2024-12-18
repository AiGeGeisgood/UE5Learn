#include "XGSampleEMailSubsystem.h"


FXGSampleEmailSubsystem* FXGSampleEmailSubsystem::XGEmailSubsystem = new FXGSampleEmailSubsystem();

FXGSampleEmailSubsystem::FXGSampleEmailSubsystem()
{
}

FXGSampleEmailSubsystem::~FXGSampleEmailSubsystem()
{
	XGEmailAsyncActions.Empty();
}

FXGSampleEmailSubsystem* FXGSampleEmailSubsystem::GetXGEmailSubsystem()
{

	return XGEmailSubsystem;
}

void FXGSampleEmailSubsystem::RealeaseXGEmailSubsystem()
{
	if (XGEmailSubsystem)
	{
		delete XGEmailSubsystem;

		XGEmailSubsystem = nullptr;
	}

}

FGuid FXGSampleEmailSubsystem::CreateEmailAsyncAction(FXGSampleEmailInfo InEmailInfo, FXGSampleEmailDelegate InEmailDelegate)
{
	TSharedPtr<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe> XGEmailAsyncAction = FXGSampleEmailAsyncAction::CreateEmailAsyncAction(InEmailInfo, InEmailDelegate);

	XGEmailAsyncActions.Add(XGEmailAsyncAction->AsyncID, XGEmailAsyncAction);

	return XGEmailAsyncAction->AsyncID;
}

TArray<FGuid> FXGSampleEmailSubsystem::GetAllEmailAsyncActionIDs()
{
	TArray<FGuid> IDs;
	XGEmailAsyncActions.GetKeys(IDs);
	return IDs;
}

bool FXGSampleEmailSubsystem::QueryPointedEmailAsyncActionStatus(FGuid InAsyncID, EXGSampleEMailAsyncActionStatus& OutActionStatus)
{
	if (TSharedPtr<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe>* EmailAsyncActionPtr = XGEmailAsyncActions.Find(InAsyncID))
	{
		OutActionStatus = (*EmailAsyncActionPtr)->GetAsyncActionStatus();

		return true;
	}
	else
	{
		OutActionStatus = EXGSampleEMailAsyncActionStatus::None;
		return false;
	}
}

bool FXGSampleEmailSubsystem::RemovePointedEmailAsyncAction(FGuid InAsyncID)
{
	return XGEmailAsyncActions.Remove(InAsyncID) >= 0;

}

void FXGSampleEmailSubsystem::RealeaseAllEmailAsyncActions()
{
	XGEmailAsyncActions.Empty();
}


