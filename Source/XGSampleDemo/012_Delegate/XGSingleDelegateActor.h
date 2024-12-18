#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGSingleDelegateActor.generated.h"



DECLARE_DELEGATE(FXGSingDelegatePrintLocation);

DECLARE_DELEGATE_RetVal_OneParam(FVector, FXGSingDelegateGetLocation, FString);



//单播

UCLASS()
class XGSAMPLEDEMO_API AXGLocationActor : public AActor
{
	GENERATED_BODY()

public:

	AXGLocationActor();

protected:

	virtual void BeginPlay() override;

	void PrintLocation();

	FVector GetLocation(FString InStr);

	FVector GetLocationWithPayload(FString InStr, int32 Health, int32 Money);

	int32 Mana = 99;

};



class FXGSingeRawStruct
{
public:

	FXGSingeRawStruct()
	{
		UE_LOG(LogTemp, Warning, TEXT("XGSingeRawStruct 构造了"));
	}

	virtual ~FXGSingeRawStruct()
	{
		UE_LOG(LogTemp, Warning, TEXT("XGSingeRawStruct 析构了"));
	}



	void RawLocation(int32 InPayloadMoney)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]__XGSingeRawStruct__%d"), *FString(__FUNCTION__), InPayloadMoney);
	}

	void SmartLocation()
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]__XGSingeRawStruct"), *FString(__FUNCTION__));
	}


};


UCLASS()
class XGSAMPLEDEMO_API AXGSingleDelegateActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGSingleDelegateActor();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallLocationActorPrint();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallLocationActorGet();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallLocationActorGetWithPayload();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfLambda();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfRaw();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfSmartPointer();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfSmartPointerLambda();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMyStaticMethod();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfSmartPointerSafe();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfUFuncion();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfUObject();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfWeakLambda();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMySelfReplace();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallReplaceMyDeleage();

	//这个代理不是动态,不能在蓝图进行使用,但是可以在C++进行使用
	//UFUNCTION(BlueprintCallable, Category = "XG")
	void CallReplaceMyDeleage2(FXGSingDelegatePrintLocation InReplaceDelegate);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MyFunction();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	FXGSingDelegatePrintLocation SingDelegatePrintLocation;

	FXGSingDelegateGetLocation SingDelegateGetLocation;

	FXGSingDelegateGetLocation SingDelegateGetLocationWithPayload;

	FXGSingDelegatePrintLocation SingDelegateLambda;

	FXGSingDelegateGetLocation SingDelegateLambdaLocation;

	FXGSingDelegatePrintLocation SingDelegateRaw;

	FXGSingDelegatePrintLocation SingDelegateSmartPointer;

	TSharedPtr<FXGSingeRawStruct> SmartStructPtr;

	FXGSingDelegatePrintLocation SingDelegateSmartPointerLambda;

	FXGSingDelegatePrintLocation SingDelegateStaticMethod;

	FXGSingDelegatePrintLocation SingDelegateSmartPointerSafe;

	FXGSingDelegatePrintLocation SingDelegateUFunction;

	FXGSingDelegatePrintLocation SingDelegateUObject;

	FXGSingDelegatePrintLocation SingDelegateWeakLambda;

	FXGSingDelegatePrintLocation SingDelegateReplace;
};
