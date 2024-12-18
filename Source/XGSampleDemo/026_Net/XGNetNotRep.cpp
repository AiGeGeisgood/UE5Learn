


#include "XGNetNotRep.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AXGNetNotRep::AXGNetNotRep()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("XGRootComponent"));


	bReplicates = true;

}

// Called when the game starts or when spawned
void AXGNetNotRep::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXGNetNotRep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{

		ReplicatedStruct.ReplicatedProperty++;

		ReplicatedStruct.NotReplicatedProperty++;
		ReplicatedStruct.NotReplicatedProperty++;
	}

	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		FString healthMessage = FString::Printf(TEXT("%s now ReplicatedProperty:[%d],NotReplicatedProperty:[%d]"), *GetFName().ToString(), 
		ReplicatedStruct.ReplicatedProperty,
		ReplicatedStruct.NotReplicatedProperty
		);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, healthMessage);
	}

}

void AXGNetNotRep::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXGNetNotRep, ReplicatedStruct);


}


