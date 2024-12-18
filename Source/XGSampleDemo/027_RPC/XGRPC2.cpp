


#include "XGRPC2.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AXGRPC2::AXGRPC2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Money = 100;

}
bool  AXGRPC2::ClientCallServcerUpdateMoney_Validate(int32 InAddMoney)
{
	if (InAddMoney>5)
	{
		return false;
	}
	return true;
}

void AXGRPC2::ClientCallServcerUpdateMoney_Implementation(int32 InAddMoney)
{

	Money += InAddMoney;

	ShowMoney();
}

// Called when the game starts or when spawned
void AXGRPC2::BeginPlay()
{
	Super::BeginPlay();
	
}

void AXGRPC2::ShowMoney()
{

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		FString Message = FString::Printf(TEXT("Server Money:%d"), Money);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	}
	else
	{
		FString Message = FString::Printf(TEXT("Client Money:%d"), Money);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Message);
	}



}

void AXGRPC2::OnRep_Money()
{

	ShowMoney();
}

// Called every frame
void AXGRPC2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXGRPC2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{	// 调用Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 添加要为派生的类复制的属性
	DOREPLIFETIME(AXGRPC2, Money);

}


