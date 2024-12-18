#pragma once

#include "CoreMinimal.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"


#include "XGSampleBDReqType.generated.h"



struct  FXGSampleBDReqUti
{

	static bool WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, const FString& InStringValue);

	static bool WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, int32 InIntValue);

	static bool WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, float InFloatValue);

	static bool WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, bool InBoolValue);

};






USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleBDReqMessageInfo
{
	GENERATED_BODY()

public:

	//[user,assistant]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString role = TEXT("user");


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString content = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString name = TEXT("None");

	void ToJsonWriter(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter);

};


USTRUCT(BlueprintType)
struct XGSAMPLEBDLINK_API FXGSampleBDReqInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	TArray<FXGSampleBDReqMessageInfo> messages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	float temperature = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	float top_p = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	float penalty_score = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool stream = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString system = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	TArray<FString> stop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool disable_search = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool enable_citation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	bool enable_trace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	int32 max_output_tokens = -1;

	FString response_format = TEXT("text");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG BaiDu Chat")
	FString user_id = TEXT("None");

	FString ToJsonString();

};