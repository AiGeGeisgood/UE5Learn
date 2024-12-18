

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGRPC.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGRPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXGRPC();


	UFUNCTION(BlueprintCallable,Client,Reliable)	
	void ClientRPCFunction(int32 InAddActualHealth);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPCFunction(int32 InAddPlanHealth);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MulticastRPCFunction(int32 InAddMultiHealth);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 要复制的属性
	UPROPERTY(Replicated)
	uint32 Health;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 重写复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
