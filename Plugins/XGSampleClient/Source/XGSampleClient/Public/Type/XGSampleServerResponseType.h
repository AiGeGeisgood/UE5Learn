#pragma once

#include "CoreMinimal.h"
#include "XGSampleServerHttpType.h"

#include "XGSampleServerResponseType.generated.h"




UENUM(BlueprintType)
enum class EXGLoginServerResponseType : uint8
{
	CheckVersionResp,

	RegisterResp,
	LoginResp,


	MAX,


};

USTRUCT(Blueprintable, BlueprintType)
struct FXGSampleServerResponse
{
	GENERATED_USTRUCT_BODY()

public:
	FXGSampleServerResponse()
		:bResult(false)
		, ResultType(EXGSampleServerResult::Failed)
		, ResponseType(EXGLoginServerResponseType::MAX)
		, Data(TEXT(""))
	{

	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	bool bResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	EXGSampleServerResult ResultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	EXGLoginServerResponseType ResponseType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	FString Data;

};


USTRUCT(Blueprintable, BlueprintType)
struct FXGRegisterRespData
{
	GENERATED_USTRUCT_BODY()

public:

	FXGRegisterRespData() :
		UserName(TEXT("None")),
		UserID(TEXT("-1")),
		RegisterTime(FDateTime())
	{ }
	virtual ~FXGRegisterRespData() { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	FString UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	FString UserID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	FDateTime RegisterTime;
};



USTRUCT(Blueprintable, BlueprintType)
struct FXGLoginRespData
{
	GENERATED_USTRUCT_BODY()

public:

	FXGLoginRespData() :
		UserName(TEXT("None")),
		UserID(TEXT("-1"))
	{ }
	virtual ~FXGLoginRespData() { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	FString UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|ResponseType")
	FString UserID;
};
