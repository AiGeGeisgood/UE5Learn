


#include "XGNetDerivedConditionRepUsing.h"

#include "Net/UnrealNetwork.h"
// Sets default values
AXGNetDerivedConditionRepUsing::AXGNetDerivedConditionRepUsing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}


void AXGNetDerivedConditionRepUsing::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 调用Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 添加带条件的属性复制
	DOREPLIFETIME_CONDITION(AXGNetDerivedConditionRepUsing, OwnerSeeInt, COND_OwnerOnly);
}

// Called when the game starts or when spawned
void AXGNetDerivedConditionRepUsing::BeginPlay()
{
	Super::BeginPlay();
	



}

// Called every frame
void AXGNetDerivedConditionRepUsing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		OwnerSeeInt++;
	}


	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		FString healthMessage1 = FString::Printf(TEXT("Server e %s now has %d health remaining"), *GetFName().ToString(), OwnerSeeInt);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, healthMessage1);

	}


}


