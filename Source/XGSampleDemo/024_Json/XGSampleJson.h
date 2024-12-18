//对象(object) int float bool str  数组

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGSampleJson.generated.h"


struct TempServerInfo {

	FString UEServerName =TEXT("");
	FString ServerVersion =TEXT("");

};
struct TempCodersInfo {

	FString Name = TEXT("");
	float WorkYear =-1.f ;

};

struct TempMessageInfo {

	int32 UECode = -1;
	FString UEMessage = TEXT("");
	FString UEData = TEXT("");
	FString Sid = TEXT("");

	TempServerInfo UEServerInfo;

	TArray<int32> WorkerIDs;

	TArray<TempCodersInfo> Coders;

	//输出自己的Json
	FString ToString();

	//另外构建的一个
	static TempMessageInfo FromString(const FString& InJsonString);


};





struct FBadMessageInfo
{
	int32 Code = -1;
	FString Message = TEXT("");
	FString Sid = TEXT("");
	FString Info = TEXT("");
};



USTRUCT()
struct FXGSampleServerInfo {


	GENERATED_BODY()

	UPROPERTY()
	FString ServerName;

	UPROPERTY()
	FString ServerVersion;

	UPROPERTY()
	FString TestInfo;

	FString NotWrite =TEXT("");

};


USTRUCT()
struct FXGSampleCoderInfo {


	GENERATED_BODY()

	UPROPERTY()
	FString Name=TEXT("None");

	UPROPERTY()
	double WorkYear=-1.f;




};

USTRUCT()
struct FXGSampleMessageInfo {


	GENERATED_BODY()

	UPROPERTY()
	int32 Code = -1;

	UPROPERTY()
	FString Message=TEXT("");

	UPROPERTY()
	FString Data = TEXT("");

	UPROPERTY()
	FString Sid = TEXT("");

	UPROPERTY() 
	FXGSampleServerInfo Info;

	UPROPERTY()
	TArray<int32> WorkerIDs;

	UPROPERTY()
	TArray<FXGSampleCoderInfo> Coders;
	
	FString NotWrite = TEXT("111");

	UPROPERTY()
	FDateTime TempTime ;

};






UCLASS()
class XGSAMPLEDEMO_API AXGSampleJson : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGSampleJson();

public:

	UFUNCTION(BlueprintCallable, Category = "XG")
	void NotGoodJson();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void GoodJson();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void GoodJson2();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void BadJson();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void VeryBadJson();



	UFUNCTION(BlueprintCallable, Category = "XG")
	void GoodSturctJson();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void GoodSturctJson2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
