#pragma once
#include "CoreMinimal.h"

#include "XGSampleServerUtil.h"
#include "XGSampleServerHttpType.h"

#include "XGSampleServerRequestType.generated.h"

UENUM(BlueprintType)
enum class EXGSampleRequestType : uint8
{
	CheckVersionReq,

	RegisterReq,
	LoginReq,

	MAX
};


USTRUCT(Blueprintable, BlueprintType)
struct FXGSampleServerRequest
{
	GENERATED_USTRUCT_BODY()
public:
	FXGSampleServerRequest()
		:RequestType(EXGSampleRequestType::MAX)
		, Data(TEXT(""))
	{

	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	EXGSampleRequestType RequestType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString Data;

};


USTRUCT(Blueprintable, BlueprintType)
struct FXGSampleServerDataBase
{
	GENERATED_USTRUCT_BODY()

public:

	FXGSampleServerDataBase() { }
	virtual ~FXGSampleServerDataBase() { }

public:

	virtual	bool IsValid(EXGSampleServerResult& OutRequestDataValidity) const { return false; };

};

USTRUCT(Blueprintable, BlueprintType)
struct FXGCheckVersionReqData :public FXGSampleServerDataBase
{
	GENERATED_USTRUCT_BODY()
public:
	FXGCheckVersionReqData()
		: ClientVersion(TEXT("1.0.0"))
	{

	}
	virtual	~FXGCheckVersionReqData()
	{

	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString ClientVersion;

	virtual	bool IsValid(EXGSampleServerResult& OutRequestDataValidity) const override
	{
		if (ClientVersion.IsEmpty())
		{
			OutRequestDataValidity = EXGSampleServerResult::IllVersion;
			return false;
		}
		OutRequestDataValidity = EXGSampleServerResult::Succeeded;
		return true;
	}
};



USTRUCT(Blueprintable, BlueprintType)
struct FXGRegisterReqData :public FXGSampleServerDataBase
{
	GENERATED_USTRUCT_BODY()

public:

	FXGRegisterReqData()
		:UserName(TEXT("-1"))
		, Password(TEXT("-1"))
		, Phone(TEXT("-1"))
		, EMail(TEXT("-1"))
	{

	}
	virtual ~FXGRegisterReqData() { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString UserName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString Phone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString EMail;

	virtual	bool IsValid(EXGSampleServerResult& OutRequestDataValidity) const override
	{

		if (!FXGLoginServerUtil::CheckUserName(UserName))
		{
			OutRequestDataValidity = EXGSampleServerResult::IllegalUserName;
			return false;
		}



		if (!FXGLoginServerUtil::CheckPassword(Password))
		{
			OutRequestDataValidity = EXGSampleServerResult::IllegaPassword;
			return false;
		}



		if (!FXGLoginServerUtil::CheckPhone(Phone))
		{
			OutRequestDataValidity = EXGSampleServerResult::IllegaPhone;
			return false;
		}

		if (!FXGLoginServerUtil::CheckEMail(EMail))
		{
			OutRequestDataValidity = EXGSampleServerResult::IllegalEMail;
			return false;
		}

		OutRequestDataValidity = EXGSampleServerResult::Succeeded;

		return true;

	}


};
USTRUCT(Blueprintable, BlueprintType)
struct FXGLoginReqData :public FXGSampleServerDataBase
{
	GENERATED_USTRUCT_BODY()

public:

	FXGLoginReqData()
		:UserName(TEXT("-1"))
		, Password(TEXT("-1"))
	{

	}
	virtual ~FXGLoginReqData() { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString UserName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGLogin|RequestType")
	FString Password;

	virtual	bool IsValid(EXGSampleServerResult& OutRequestDataValidity) const override
	{


		if (!FXGLoginServerUtil::CheckUserName(UserName))
		{
			OutRequestDataValidity = EXGSampleServerResult::IllegalUserName;
			return false;
		}



		if (!FXGLoginServerUtil::CheckPassword(Password))
		{
			OutRequestDataValidity = EXGSampleServerResult::IllegaPassword;
			return false;
		}

		OutRequestDataValidity = EXGSampleServerResult::Succeeded;
		return true;
	}



};
