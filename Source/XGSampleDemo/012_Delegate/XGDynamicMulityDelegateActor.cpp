#include "XGDynamicMulityDelegateActor.h"

// Sets default values
AXGDynamicMulityDelegateActor::AXGDynamicMulityDelegateActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGDynamicMulityDelegateActor::InitXGDynamicMultiThree(FXGDynamicMultiThree InDelegate)
{
	XGDynamicMultiThree= InDelegate;
}

void AXGDynamicMulityDelegateActor::CallExceteDynamicMultiThreeRight(FString InName, int32 InHealth, int32 InMana)
{
	XGDynamicMultiThreeRight.Broadcast(InName, InHealth, InMana);

}

void AXGDynamicMulityDelegateActor::WorkDynamicNative(FString InName, int32 InHealth, int32 InMana)
{

	UE_LOG(LogTemp,Warning,TEXT("WorkDynamicNative"));

} 

// Called when the game starts or when spawned
void AXGDynamicMulityDelegateActor::BeginPlay()
{
	Super::BeginPlay();

	XGDynamicMultiThreeRight.AddDynamic(this, &AXGDynamicMulityDelegateActor::WorkDynamicNative);

	XGDynamicMultiThreeRight.RemoveDynamic(this, &AXGDynamicMulityDelegateActor::WorkDynamicNative);


}

// Called every frame
void AXGDynamicMulityDelegateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
