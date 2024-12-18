#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
#include "XGCountDownTimerActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGCountDownTimerActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGCountDownTimerActor();

	class UTextRenderComponent* CountdownText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AdvanceTimer();

	void UpdateTimerDisplay();

	UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();

	


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	UPROPERTY(EditAnywhere)
	int32 CountdownTime ;

	FTimerHandle CountdownTimerHandle;

};
