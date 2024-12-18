#include "TimerActor.h"

// Sets default values
ATimerActor::ATimerActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimerActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerManager& ThisTimeManager = GetWorldTimerManager();

	ThisTimeManager.SetTimer(MyTimerHandle, this, &ATimerActor::RepeatingFunction, 1.0f, true, 2.0f);


}

// Called every frame
void ATimerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	GetWorldTimerManager().IsTimerActive(MyTimerHandle);

	GetWorldTimerManager().GetTimerRate(MyTimerHandle);

	GetWorldTimerManager().GetTimerElapsed(MyTimerHandle);


}

void ATimerActor::RepeatingFunction()
{
	if (RepeatingCallsRemaining<0)
	{
		GetWorldTimerManager().ClearTimer(MyTimerHandle);

		UE_LOG(LogTemp,Warning,TEXT("Timer End!"));
	}
	else
	{	
		//工作
		--RepeatingCallsRemaining;
		UE_LOG(LogTemp, Warning, TEXT("Timer Working! Remained Num: %d"), RepeatingCallsRemaining);

		
	}



}
