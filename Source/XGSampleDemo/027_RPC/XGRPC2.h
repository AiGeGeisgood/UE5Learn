

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGRPC2.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGRPC2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGRPC2();


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ClientCallServcerUpdateMoney(int32 InAddMoney);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 要复制的属性
	UPROPERTY(ReplicatedUsing = OnRep_Money)
	uint32 Money;

	void ShowMoney();

public:	

	// 不传递参数的签名
	UFUNCTION()
	void OnRep_Money();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
