

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/CoreNet.h"
#include "XGNetDerivedConditionRepUsingAN.generated.h"


UCLASS()
class XGSAMPLEDEMO_API AXGNetDerivedConditionRepUsingAN : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGNetDerivedConditionRepUsingAN();



	// 要复制的属性
	UPROPERTY(ReplicatedUsing = OnRep_Always)
	uint32 AlwaysInt;

	// 要复制的属性
	UPROPERTY(ReplicatedUsing = OnRep_OnChanged)
	uint32 OnChangedInt;


	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;


	bool IsInvincible();

	// 不传递参数的签名
	UFUNCTION()
	void OnRep_Always();


	// 不传递参数的签名
	UFUNCTION()
	void OnRep_OnChanged();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
