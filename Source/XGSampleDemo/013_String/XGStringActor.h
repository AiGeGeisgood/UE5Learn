#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGStringActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGStringActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGStringActor();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void InitString();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void PropertyString();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ModifyString();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ModifyString2();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void LogString();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void OperateString();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void OperateString2();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void OperateURL();

	//问题很大 这样写!!!
	UFUNCTION(BlueprintCallable, Category = "XG")
	void TransformString();

	void WrongUse(char* InStr);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void TransformStringRight();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void FNameTest();

	UFUNCTION(BlueprintCallable, Category = "XG")
	FText FTextTest();

	UFUNCTION(BlueprintCallable, Category = "XG")
	FText FTextFormat();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
