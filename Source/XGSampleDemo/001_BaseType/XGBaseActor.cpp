#include "XGBaseActor.h"

// Sets default values
AXGBaseActor::AXGBaseActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AXGBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
