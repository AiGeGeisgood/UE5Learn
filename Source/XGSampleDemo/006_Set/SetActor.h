#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SetActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API ASetActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASetActor();



	UFUNCTION(BlueprintCallable)
	void InitSet();

	UFUNCTION(BlueprintCallable)
	void LoopSet();

	UFUNCTION(BlueprintCallable)
	void QuerySet();

	UFUNCTION(BlueprintCallable)
	void RemoveSet();

	UFUNCTION(BlueprintCallable)
	void SortSet();

	UFUNCTION(BlueprintCallable)
	void OpeatorSet();

	UFUNCTION(BlueprintCallable)
	void SlackSet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = SetExample)
	TSet<FString> MyFruitSet;

};
