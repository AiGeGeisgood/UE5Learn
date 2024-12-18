#pragma once

#include "CoreMinimal.h"
#include "XGSampleEMailType.generated.h"


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FXGSampleEmailDelegate, FGuid, AsyncID, bool, bResult, FString, Message);


UENUM(BlueprintType)
enum class EXGSampleEMailAsyncActionStatus :uint8
{
	None,
	Init,
	Connected,
	ConnectedSuccess,
	ConnectedError,
	EHLO,
	AuthLogin_UserName,
	AuthLogin_Password,
	AuthLogin_Result,
	SetMailFrom,
	SetEmailTo,
	DataPrepare,
	Data,
	Quit,
	Finished,
	Closed,
	UnKnowedError,
	Max,
};



USTRUCT(BlueprintType)
struct XGSAMPLEEMAIL_API FXGSampleEmailAuthInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString UserName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString AuthCode = TEXT("");

};



USTRUCT(BlueprintType)
struct XGSAMPLEEMAIL_API FXGSampleEmailContentInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString FromName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString FromEmail = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString ToName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString ToEmail = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString Subject = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FString Content = TEXT("");

	FString ToString();


};


USTRUCT(BlueprintType)
struct XGSAMPLEEMAIL_API FXGSampleEmailInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FXGSampleEmailAuthInfo AuthInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGEmail")
	FXGSampleEmailContentInfo ContentInfo;

};
