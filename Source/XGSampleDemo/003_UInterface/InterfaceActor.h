#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceActor.generated.h"


UINTERFACE(MinimalAPI)
class UXGHealthInterface : public UInterface
{
	GENERATED_BODY()


};


class XGSAMPLEDEMO_API IXGHealthInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool IsAlive();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDeath();

};

UENUM(BlueprintType)
enum class EXGActorType:uint8
{
	None UMETA(DisplayName = "未初始化"),
	NPC,
	Player,
	Max UMETA(Hidden)
};




UINTERFACE(MinimalAPI)
class UXGActorTypeInterface : public UInterface
{
	GENERATED_BODY()

public:



};


class XGSAMPLEDEMO_API IXGActorTypeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	EXGActorType GetActorType();

};





UCLASS()
class XGSAMPLEDEMO_API AInterfaceActor : public AActor, public IXGHealthInterface,public IXGActorTypeInterface
{
	GENERATED_BODY()

public:

	AInterfaceActor();

public:

	bool IsDeath_Implementation() override;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
};
