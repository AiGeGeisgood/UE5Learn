// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGSampleEMailBPLibrary.h"
#include "XGSampleEmailSubsystem.h"
#include "XGSampleEMail.h"

void UXGSampleEMailBPLibrary::XGEmailSimpleSend(FXGSampleEmailInfo InEmailInfo, FXGSampleEmailDelegate InEmailDelegate, FGuid& OutAsyncID)
{

	FXGSampleEmailSubsystem* XGEmailSubsystem = FXGSampleEmailSubsystem::GetXGEmailSubsystem();

	OutAsyncID = XGEmailSubsystem->CreateEmailAsyncAction(InEmailInfo, InEmailDelegate);


}

TArray<FGuid> UXGSampleEMailBPLibrary::XGGetAllEmailAsyncActionIDs()
{
	FXGSampleEmailSubsystem* XGEmailSubsystem = FXGSampleEmailSubsystem::GetXGEmailSubsystem();

	return XGEmailSubsystem->GetAllEmailAsyncActionIDs();

}

void UXGSampleEMailBPLibrary::XGQueryPointedEmailAsyncActionStatus(FGuid InAsyncID, bool& bFind, EXGSampleEMailAsyncActionStatus& OutActionStatus)
{
	FXGSampleEmailSubsystem* XGEmailSubsystem = FXGSampleEmailSubsystem::GetXGEmailSubsystem();

	bFind = XGEmailSubsystem->QueryPointedEmailAsyncActionStatus(InAsyncID, OutActionStatus);



}

bool UXGSampleEMailBPLibrary::XGRemovePointedEmailAsyncAction(FGuid InAsyncID)
{
	FXGSampleEmailSubsystem* XGEmailSubsystem = FXGSampleEmailSubsystem::GetXGEmailSubsystem();

	return 	XGEmailSubsystem->RemovePointedEmailAsyncAction(InAsyncID);
}

void UXGSampleEMailBPLibrary::XGRealeaseAllEmailAsyncActions()
{
	FXGSampleEmailSubsystem* XGEmailSubsystem = FXGSampleEmailSubsystem::GetXGEmailSubsystem();

	XGEmailSubsystem->RealeaseAllEmailAsyncActions();

}
