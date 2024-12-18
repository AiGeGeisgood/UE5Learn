#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGDynamicMulityDelegateActor.generated.h"
//动态多播

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FXGDynamicMultiThree,FString,InName,int32,InHealth,int32,InMana);

UCLASS()
class XGSAMPLEDEMO_API AXGDynamicMulityDelegateActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGDynamicMulityDelegateActor();

	//骚操作
	void InitXGDynamicMultiThree(FXGDynamicMultiThree InDelegate);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallExceteDynamicMultiThreeRight(FString InName, int32 InHealth, int32 InMana);

	UFUNCTION()
	void WorkDynamicNative(FString InName, int32 InHealth, int32 InMana);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FXGDynamicMultiThree XGDynamicMultiThree;

	UPROPERTY(BlueprintAssignable)
	FXGDynamicMultiThree XGDynamicMultiThreeRight;

};
