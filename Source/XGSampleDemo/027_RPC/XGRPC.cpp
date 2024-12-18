


#include "XGRPC.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AXGRPC::AXGRPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Health =100;

}

void AXGRPC::ClientRPCFunction_Implementation(int32 InAddActualHealth)
{
	FString healthMessage = FString::Printf(TEXT("Own Client  %s now Add Health %d "), *GetFName().ToString(), InAddActualHealth);
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, healthMessage);

}

void AXGRPC::ServerRPCFunction_Implementation(int32 InAddPlanHealth)
{
	
	Health = Health + InAddPlanHealth-10;

	FString healthMessage = FString::Printf(TEXT("Server  %s now Add Health %d "), *GetFName().ToString(), InAddPlanHealth);
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, healthMessage);

	ClientRPCFunction(InAddPlanHealth - 10);

	MulticastRPCFunction((InAddPlanHealth - 10));


}

void AXGRPC::MulticastRPCFunction_Implementation(int32 InAddMultiHealth)
{

	FString healthMessage = FString::Printf(TEXT("Multi Client  %s now Add Health %d "), *GetFName().ToString(), InAddMultiHealth);
	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, healthMessage);

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
	 UE_LOG(LogTemp,Warning,TEXT("这是服务器 受到了广播"));

	}

	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("这是客户端 受到了广播"));

	}


}

// Called when the game starts or when spawned
void AXGRPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXGRPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXGRPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 调用Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 添加要为派生的类复制的属性
	DOREPLIFETIME(AXGRPC, Health);

}


