#include "XGPluginActor.h"

// Sets default values
AXGPluginActor::AXGPluginActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AXGPluginActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGPluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
