#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGBaseActor.generated.h"


UCLASS()
class XGSAMPLEDEMO_API AXGBaseActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

