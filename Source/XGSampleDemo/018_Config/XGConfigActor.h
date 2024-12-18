#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGConfigActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGConfigActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGConfigActor();


public:

	UFUNCTION(BlueprintPure, Category = "XG")
	bool GetMyAppKey(FString& OutMyAppKey );

	UFUNCTION(BlueprintCallable, Category = "XG")
	void SetMyAppKey(const FString& InMyAppKey);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
