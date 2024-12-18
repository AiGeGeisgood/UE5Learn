#include "XGClassActor.h"

// Sets default values
AXGClassActor::AXGClassActor()
	:Money(200)
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));

	Money = 400;

}

int32 AXGClassActor::GetCurrentMoney()
{
	return Money;
}

int32 AXGClassActor::GetMyCDOMoney()
{
	UClass* XGClass = AXGClassActor::StaticClass();

	UObject* MyObject = XGClass->GetDefaultObject();

	AXGClassActor* MyCDO = Cast<AXGClassActor>(MyObject);

	if (MyCDO)
	{
		return MyCDO->Money;
	}


	return -1;
}



// Called when the game starts or when spawned
void AXGClassActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGClassActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
