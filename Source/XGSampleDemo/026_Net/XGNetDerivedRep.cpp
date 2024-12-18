


#include "XGNetDerivedRep.h"
#include "Net/UnrealNetwork.h"


AXGNetDerivedRep::AXGNetDerivedRep()
{
	RootComponent =CreateDefaultSubobject<USceneComponent>(TEXT("XGRootComponent"));


	bReplicates = true;
}

void AXGNetDerivedRep::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 调用Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 添加要为派生的类复制的属性
	DOREPLIFETIME(AXGNetDerivedRep, Health);
}

void AXGNetDerivedRep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{

		Health++;

	} 

	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %d health remaining."), *GetFName().ToString(), Health);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, healthMessage);
	}

}
