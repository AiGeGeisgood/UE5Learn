

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGNetDerivedRep.generated.h"



UCLASS()
class XGSAMPLEDEMO_API AXGNetDerivedRep : public AActor
{
	GENERATED_BODY()
	
public:	


	// 要复制的属性
	UPROPERTY(Replicated)
	uint32 Health;


	// 派生的Actor构造函数 
	AXGNetDerivedRep();

	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
