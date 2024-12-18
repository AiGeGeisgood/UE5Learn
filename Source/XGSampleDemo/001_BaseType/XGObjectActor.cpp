#include "XGObjectActor.h"


UXGObjectObject::UXGObjectObject()
{

 UE_LOG(LogTemp,Warning,TEXT("啊哈,我被初始化了"));

}

UXGObjectObject::UXGObjectObject(const FObjectInitializer& ObjectInitializer)
{

}

UXGObjectObject::~UXGObjectObject()
{


	UE_LOG(LogTemp, Warning, TEXT("啊哈,我被回收了"));


}





// Sets default values
AXGObjectActor::AXGObjectActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
}

// Called when the game starts or when spawned
void AXGObjectActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXGObjectActor::InitMyXGObject()
{
	MyXGObject = NewObject<UXGObjectObject>();

	//MyXGObject->AddToRoot();

	//MyXGObject->RemoveFromRoot();


	//UXGObjectObject MyObject2 = new UXGObjectObject();
	//UXGObjectObject MyObject3;
}

int32 AXGObjectActor::GetMyXGObjectHealth()
{
	if (MyXGObject)
	{
		return MyXGObject->Health;
	}

	return  -1;
}

