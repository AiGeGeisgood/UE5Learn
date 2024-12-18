#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGFunctionActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGFunctionActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGFunctionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintPure)

	float  BlueprintPureFunction();

	UFUNCTION(BlueprintCallable)

	float BlueprintCallableFunction();



	UFUNCTION(BlueprintCallable)

	int32 BlueprintCallableConstFunction() const;



	UFUNCTION(BlueprintPure = fasle)

	int32 BlueprintPureFalseFunction() const;

public:


	UFUNCTION(BlueprintPure)
	int32 PureGetCurrentMoney();

	UFUNCTION(BlueprintCallable)
	int32 CallableGetCurrentMoney();

	UFUNCTION(BlueprintPure)
	int32 PureGetLastCurrentMoney() ;

	UFUNCTION(BlueprintCallable)
	int32 CallableGetLastCurrentMoney();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	int32 BPOnlyMustOverride(float InMyFloat);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 BPAndCPPMustOverride(float InMyFloat);

public:
	UFUNCTION(CallInEditor)
	void DetailCallInEditor();

	UFUNCTION(BlueprintCallable)
	bool GetBodyStauts(const int32& InMyNum, int32& OutMyNum,int32 InNorNum);

	UFUNCTION(BlueprintCallable)
	bool GetBodyArray(UPARAM(ref) TArray<int32>& InNum);

	UFUNCTION(BlueprintCallable)
	bool GetBodyArray_Error(TArray<int32> InNum);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Money = 10;



};
