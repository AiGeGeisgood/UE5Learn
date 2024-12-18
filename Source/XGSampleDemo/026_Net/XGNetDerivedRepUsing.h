

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGNetDerivedRepUsing.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGNetDerivedRepUsing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGNetDerivedRepUsing();



	// 使用OnRep_Value复制的属性
	UPROPERTY(ReplicatedUsing = OnRep_Value)
	int32 HealthValue1;

	// 使用OnRep_ConstRef复制的属性
	UPROPERTY(ReplicatedUsing = OnRep_ConstRef)
	int32 HealthValue2;

	// 使用OnRep_NoParam复制的属性
	UPROPERTY(ReplicatedUsing = OnRep_NoParam)
	int32 HealthValue3;



	// 要传递最后一个值的副本的签名
	UFUNCTION()
	void OnRep_Value(int32 LastHealthValue);

	// 要传递常量引用的签名
	UFUNCTION()
	void OnRep_ConstRef(const int32& LastHealthValue);

	// 不传递参数的签名
	UFUNCTION()
	void OnRep_NoParam();

	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
