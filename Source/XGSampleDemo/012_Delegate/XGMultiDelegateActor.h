#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGMultiDelegateActor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FXGMulityDelegate,FString)

UCLASS()
class XGSAMPLEDEMO_API AXGMultiExecuteActor : public AActor
{
	GENERATED_BODY()

public:
	AXGMultiExecuteActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
 
	void Work(FString InFString, int32 InHealth);

public:
	
	FGuid MyActorID ;

protected:
	FDelegateHandle MyDelegateHandle;
	
};


class FXGMultiRawStructSafe :public TSharedFromThis<FXGMultiRawStructSafe,ESPMode::ThreadSafe>
{

public:

	FXGMultiRawStructSafe()
	{
		UE_LOG(LogTemp, Warning, TEXT("FXGMultiRawStructSafe 构造了"));
	}

	virtual ~FXGMultiRawStructSafe()
	{
		UE_LOG(LogTemp, Warning, TEXT("FXGMultiRawStructSafe 析构了"));
	}

	void PrintSPStr(FString InStr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPSafe,InStr:[%s],"), *InStr);

	}
};

class FXGMultiRawStruct :public TSharedFromThis<FXGMultiRawStruct>
{
public:

	FXGMultiRawStruct()
	{
		UE_LOG(LogTemp, Warning, TEXT("XGMultiRawStruct 构造了"));
	}

	virtual ~FXGMultiRawStruct()
	{
		UE_LOG(LogTemp, Warning, TEXT("XGMultiRawStruct 析构了"));
	}

	void PrintStr(FString InStr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Raw,InStr:[%s],"), *InStr);

	}

	void PrintSPStr(FString InStr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SP,InStr:[%s],"), *InStr);

	}

	void InitDelegate(class AXGMultiDelegateActor* InActor);


};




//多播
UCLASS()
class XGSAMPLEDEMO_API AXGMultiDelegateActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGMultiDelegateActor();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallSimpleMulti();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallMoreMulti();

	UFUNCTION()
	void MyUFUNCTION(FString InStr);

	void PrintInStr(FString InStr);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	FXGMulityDelegate MulityDelegate;

	FXGMulityDelegate MulityMoreDelegate;

protected:

	FXGMultiRawStruct* RawStruct =nullptr;

	TSharedPtr<FXGMultiRawStruct> SmartPtr;

	TSharedPtr<FXGMultiRawStructSafe,ESPMode::ThreadSafe> SmartPtrSafe;
};
