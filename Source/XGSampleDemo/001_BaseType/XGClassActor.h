#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGClassActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGClassActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGClassActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGClass")
	int32 Money = 100;

	UFUNCTION(BlueprintCallable,Category = "XGClass")
	int32 GetCurrentMoney();
	
	UFUNCTION(BlueprintCallable, Category = "XGClass")
	int32 GetMyCDOMoney();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGClass")
	int32 UnInitNum ;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGClass")
	double UnInitDouble;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGClass")
	FString UnInitStr;


	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "XGClass")
	TSubclassOf<AXGClassActor>  MyClass;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
