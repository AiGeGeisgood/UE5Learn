#pragma once

#include "CoreMinimal.h"

#include "XGSampleBDRespType.generated.h"






USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API  FXGSampleRespSearchResult
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 index = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	url = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	title = TEXT("");
};


USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleRespSearchInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	TArray<FXGSampleRespSearchResult> search_results;

};

USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleRespUsageInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 prompt_tokens = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 completion_tokens = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 total_tokens = -1;

};

USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleReapHeaderInfo
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 XRatelimitLimitRequests = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 XRatelimitLimitTokens = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 XRatelimitRemainingRequests = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 XRatelimitRemainingTokens = -1;

};





USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleRespNoSteamFinalMessage
{
	GENERATED_BODY()


public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	object = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 created = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool is_truncated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString finish_reason = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespSearchInfo search_info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString result = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool need_clear_history = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 flag = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 ban_round = -999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespUsageInfo usage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleReapHeaderInfo HeaderInfo;

};

USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleRespSteamMiddleMessage
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	object = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 created = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 sentence_id = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool is_end = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool is_truncated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString finish_reason = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespSearchInfo search_info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString result = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool need_clear_history = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 flag = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 ban_round = -999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespUsageInfo usage;


};


USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleRespSteamFinalMessage
{
	GENERATED_BODY()



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString TotalString = TEXT("");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleReapHeaderInfo HeaderInfo;

};




USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleRespErrorInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 error_code = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString	error_msg = TEXT("");

};


USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleBDRespInfo
{
	GENERATED_USTRUCT_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool bResult = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool bReqStream = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString Message = TEXT("");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespNoSteamFinalMessage NoSteamFinalMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespSteamMiddleMessage SteamMiddleMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespSteamFinalMessage SteamFinalMessage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FXGSampleRespErrorInfo ErrorMessage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString RawMessage = TEXT("");
};