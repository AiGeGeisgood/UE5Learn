#include "InterfaceActor.h"

// Sets default values
AInterfaceActor::AInterfaceActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AInterfaceActor::IsDeath_Implementation()
{
	return false;
}


// Called when the game starts or when spawned
void AInterfaceActor::BeginPlay()
{
	Super::BeginPlay();


	bool bIsImplemented = this->GetClass()->ImplementsInterface(UXGHealthInterface::StaticClass()); 



	bIsImplemented = this->Implements<UXGHealthInterface>(); 



	IXGHealthInterface* ReactingObject = Cast<IXGHealthInterface>(this); // 如果OriginalObject实现了UReactToTriggerInterface，则ReactingObject将为非空。



	IXGHealthInterface* RObject = Cast<IXGHealthInterface>(ReactingObject); // 如果接口被实现，则ReactingObject将为非空。

	IXGActorTypeInterface* DifferentInterface = Cast<IXGActorTypeInterface>(RObject); // 如果ReactingObject为非空而且还实现了ISomeOtherInterface，则DifferentInterface将为非空。

	AActor* Actor = Cast<AActor>(ReactingObject); // 如果ReactingObject为非空且OriginalObject为AActor或AActor派生的类，则Actor将为非空。


}

// Called every frame
void AInterfaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

