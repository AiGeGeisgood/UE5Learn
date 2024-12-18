#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGObjectActor.generated.h"



/**
 *
 */
UCLASS(BlueprintType)
class XGSAMPLEDEMO_API UXGObjectObject : public UObject
{
	GENERATED_BODY()

public:

	UXGObjectObject();


	UXGObjectObject(const FObjectInitializer& ObjectInitializer);


	virtual ~UXGObjectObject();

	int32 Health = 20; 


};



UCLASS()
class XGSAMPLEDEMO_API AXGObjectActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGObjectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)

	void InitMyXGObject();

	UFUNCTION(BlueprintCallable)
	int32 GetMyXGObjectHealth();

	UPROPERTY()
	UXGObjectObject* MyXGObject = nullptr;

};
