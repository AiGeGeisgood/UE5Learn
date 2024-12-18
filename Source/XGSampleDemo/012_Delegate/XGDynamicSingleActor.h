#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGDynamicSingleActor.generated.h"
//动态单播

DECLARE_DYNAMIC_DELEGATE_TwoParams(FXGDynamicTwo,FString,InName,int32,InMoney);


DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(int32,FXGDynamicRetOne,FString,InName);

UCLASS()
class XGSAMPLEDEMO_API AXGDynamicSingleActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGDynamicSingleActor();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void InitXGDynamicTwoDelegate(FXGDynamicTwo InDelegate);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallXGDynamicTwoDelegate(FString InStr,int32 InMoney);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ReleaseXGDynamicTwoDelegate();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void InitXGDynamicOneDelegate(FXGDynamicRetOne InDelegate);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallXGDynamicOneDelegate(FString InStr);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ReleaseXGDynamicOneDelegate();





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	FXGDynamicTwo  XGDynamicTwoDelegate;

	FXGDynamicRetOne XGDynamicRetOneDelegate;
};
