#include "XGThreadActor.h"
#include "Async/TaskGraphInterfaces.h"
// Sets default values
AXGThreadActor::AXGThreadActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AXGThreadActor::BeginPlay()
{
	Super::BeginPlay();



}

// Called every frame
void AXGThreadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
