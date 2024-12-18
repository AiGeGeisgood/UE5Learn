#include "XGPropertyActor.h"

// Sets default values
AXGPropertyActor::AXGPropertyActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AXGPropertyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGPropertyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXGPropertyActor::MyFunction(int32 BasicBitsParam)
{

	UE_LOG(LogTemp,Warning,TEXT("This Function : %d"), BasicBitsParam);

}

void AXGPropertyActor::MyOtherFunction(int32 ColorFlagsParam)
{
	UE_LOG(LogTemp, Warning, TEXT("This Function : %d"), ColorFlagsParam);
}