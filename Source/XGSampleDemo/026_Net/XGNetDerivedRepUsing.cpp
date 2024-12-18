


#include "XGNetDerivedRepUsing.h"

#include "Net/UnrealNetwork.h"
// Sets default values
AXGNetDerivedRepUsing::AXGNetDerivedRepUsing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("XGRootComponent"));
}

void AXGNetDerivedRepUsing::OnRep_Value(int32 LastHealthValue)
{
	FString healthMessage = FString::Printf(TEXT("OnRep_Value %s now has %d health remaining.Last Health:%d"), *GetFName().ToString(), HealthValue1, LastHealthValue);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, healthMessage);
}

void AXGNetDerivedRepUsing::OnRep_ConstRef(const int32& LastHealthValue)
{
	FString healthMessage = FString::Printf(TEXT("OnRep_ConstRef %s now has %d health remaining.Last Health%d"), *GetFName().ToString(), HealthValue2, LastHealthValue);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, healthMessage);
}

void AXGNetDerivedRepUsing::OnRep_NoParam()
{
	FString healthMessage = FString::Printf(TEXT("OnRep_NoParam %s now has %d health remaining."), *GetFName().ToString(), HealthValue3);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, healthMessage);
}

void AXGNetDerivedRepUsing::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	// 调用Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 添加要为派生的类复制的属性
	DOREPLIFETIME(AXGNetDerivedRepUsing, HealthValue1);
	DOREPLIFETIME(AXGNetDerivedRepUsing, HealthValue2);
	DOREPLIFETIME(AXGNetDerivedRepUsing, HealthValue3);

}

// Called when the game starts or when spawned
void AXGNetDerivedRepUsing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXGNetDerivedRepUsing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{

		HealthValue1++;

		HealthValue2++;
		HealthValue2++;

		HealthValue3++;
		HealthValue3++;
		HealthValue3++;

		FString healthMessage1 = FString::Printf(TEXT("Server OnRep_Value %s now has %d health remaining.Last Health"), *GetFName().ToString(), HealthValue1);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, healthMessage1);

		FString healthMessage2 = FString::Printf(TEXT("Server OnRep_ConstRef %s now has %d health remaining.Last Health"), *GetFName().ToString(), HealthValue2);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, healthMessage2);


		FString healthMessage3 = FString::Printf(TEXT("Server OnRep_NoParam %s now has %d health remaining."), *GetFName().ToString(), HealthValue3);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, healthMessage3);
	}


}


