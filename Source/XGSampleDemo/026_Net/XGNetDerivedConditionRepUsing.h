

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGNetDerivedConditionRepUsing.generated.h"


UCLASS()
class XGSAMPLEDEMO_API AXGNetDerivedConditionRepUsing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGNetDerivedConditionRepUsing();


	// 要复制的属性
	UPROPERTY(Replicated)
	uint32 OwnerSeeInt;

	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
