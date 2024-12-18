

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGNetNotRep.generated.h"
USTRUCT()
struct FXGNotRepStruct
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ReplicatedProperty;

	// 不复制，尽管复制了它所在的结构体
	UPROPERTY(NotReplicated)
	int32 NotReplicatedProperty;
};

UCLASS()
class XGSAMPLEDEMO_API AXGNetNotRep : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGNetNotRep();


public:

	UPROPERTY(Replicated)
	FXGNotRepStruct ReplicatedStruct;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;




	
};
