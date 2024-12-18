


#include "XGNetDerivedConditionRepUsingAN.h"
#include "Net/Core/PropertyConditions/PropertyConditions.h"
#include "Net/Core/PropertyConditions/RepChangedPropertyTracker.h"
#include "Engine/NetSerialization.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AXGNetDerivedConditionRepUsingAN::AXGNetDerivedConditionRepUsingAN()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}


void AXGNetDerivedConditionRepUsingAN::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 调用Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 添加带条件的属性复制
	/** 	此项用于总是执行RepNotify
	*	在每次复制属性时在客户端上调用关联的OnRep
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(AXGNetDerivedConditionRepUsingAN, AlwaysInt, COND_Custom,REPNOTIFY_Always);

	/** 	此项用于仅在属性更改时执行RepNotify
	*		仅在属性更改时在客户端上调用关联的OnRep
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(AXGNetDerivedConditionRepUsingAN, OnChangedInt, COND_Custom, REPNOTIFY_OnChanged);


}

void AXGNetDerivedConditionRepUsingAN::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	// 调用Super
	Super::PreReplication(ChangedPropertyTracker);


	DOREPLIFETIME_ACTIVE_OVERRIDE(AXGNetDerivedConditionRepUsingAN, AlwaysInt, IsInvincible());

	DOREPLIFETIME_ACTIVE_OVERRIDE(AXGNetDerivedConditionRepUsingAN, OnChangedInt, IsInvincible());
}

bool AXGNetDerivedConditionRepUsingAN::IsInvincible()
{
	return false;
}

void AXGNetDerivedConditionRepUsingAN::OnRep_Always()
{
	FString AlwaysMessage = FString::Printf(TEXT("%s now has %d AlwaysInt"), *GetFName().ToString(), AlwaysInt);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, AlwaysMessage);

}

void AXGNetDerivedConditionRepUsingAN::OnRep_OnChanged()
{
	FString OnChangedMessage = FString::Printf(TEXT("%s now has %d OnChangedInt"), *GetFName().ToString(), OnChangedInt);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, OnChangedMessage);

}

// Called when the game starts or when spawned
void AXGNetDerivedConditionRepUsingAN::BeginPlay()
{
	Super::BeginPlay();
	



}

// Called every frame
void AXGNetDerivedConditionRepUsingAN::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		AlwaysInt = 10;

		OnChangedInt++;

	}





}


