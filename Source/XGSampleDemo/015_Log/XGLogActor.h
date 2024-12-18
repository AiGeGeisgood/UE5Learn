#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGLogActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGLogActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGLogActor();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void  XGLog();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void  XGLogString();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void  XGLogFormatString();

	static void PringLogString(FString InStr);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
