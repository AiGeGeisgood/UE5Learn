#include "XGDynamicSingleActor.h"

// Sets default values
AXGDynamicSingleActor::AXGDynamicSingleActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGDynamicSingleActor::InitXGDynamicTwoDelegate(FXGDynamicTwo InDelegate)
{
	XGDynamicTwoDelegate= InDelegate;
}

void AXGDynamicSingleActor::CallXGDynamicTwoDelegate(FString InStr, int32 InMoney)
{

	XGDynamicTwoDelegate.ExecuteIfBound(InStr, InMoney);

}

void AXGDynamicSingleActor::ReleaseXGDynamicTwoDelegate()
{
	XGDynamicTwoDelegate.Clear();
}


void AXGDynamicSingleActor::InitXGDynamicOneDelegate(FXGDynamicRetOne InDelegate)
{
	XGDynamicRetOneDelegate = InDelegate;

	
}

void AXGDynamicSingleActor::CallXGDynamicOneDelegate(FString InStr)
{
	XGDynamicRetOneDelegate.Execute(InStr);
}

void AXGDynamicSingleActor::ReleaseXGDynamicOneDelegate()
{
	XGDynamicRetOneDelegate.Clear();
}



// Called when the game starts or when spawned
void AXGDynamicSingleActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGDynamicSingleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
