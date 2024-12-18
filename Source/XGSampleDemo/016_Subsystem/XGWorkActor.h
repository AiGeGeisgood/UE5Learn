#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGWorkActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGWorkActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGWorkActor();

public:


	UFUNCTION(BlueprintCallable, Category = "XG", meta = (WorldContext = "InWorldContextObject"))
	void ObjectGetSubsystem(const UObject* InWorldContextObject);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
