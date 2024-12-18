// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "XGSampleEMailType.h"
#include "XGSampleEMailBPLibrary.generated.h"


UCLASS()
class XGSAMPLEEMAIL_API UXGSampleEMailBPLibrary : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Send Email", Keywords = "XG Email AsyncAction Simple Send"), Category = "XGEmail")
	static void XGEmailSimpleSend(FXGSampleEmailInfo InEmailInfo, FXGSampleEmailDelegate InEmailDelegate, FGuid& OutAsyncID);

	UFUNCTION(Blueprintpure, meta = (DisplayName = "Get All Email AsyncAction IDs", Keywords = "XG Email AsyncAction ID"), Category = "XGEmail")
	TArray<FGuid> XGGetAllEmailAsyncActionIDs();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Query Pointed Email AsyncAction Status", Keywords = "XG Email AsyncAction Query Pointed Status"), Category = "XGEmail")
	void XGQueryPointedEmailAsyncActionStatus(FGuid InAsyncID, bool& bFind, EXGSampleEMailAsyncActionStatus& OutActionStatus);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove Pointed Email AsyncAction", Keywords = "XG Email AsyncAction Remove Pointed"), Category = "XGEmail")
	bool XGRemovePointedEmailAsyncAction(FGuid InAsyncID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Realease All Email AsyncActions", Keywords = "XG Email AsyncAction Realease All"), Category = "XGEmail")
	void XGRealeaseAllEmailAsyncActions();

};
