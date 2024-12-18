#include "XGWorkActor.h"
#include "XGSimpleSubsystem.h"
// Sets default values
AXGWorkActor::AXGWorkActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGWorkActor::ObjectGetSubsystem(const UObject* InWorldContextObject)
{
	if (InWorldContextObject&& InWorldContextObject->GetWorld())
	{
		UGameInstance* MyGameInstance = InWorldContextObject->GetWorld()->GetGameInstance();

		if (MyGameInstance)
		{
			UXGSimpleSubsystem* MyXGSimpleSubsystem = MyGameInstance->GetSubsystem<UXGSimpleSubsystem>();

			MyXGSimpleSubsystem->AddHealth(2000);
		}
	}

}

// Called when the game starts or when spawned
void AXGWorkActor::BeginPlay()
{
	Super::BeginPlay();


	UGameInstance* MyGameInstance = GetGameInstance();

	if (MyGameInstance)
	{
		UXGSimpleSubsystem* MyXGSimpleSubsystem = MyGameInstance->GetSubsystem<UXGSimpleSubsystem>();

		MyXGSimpleSubsystem->AddHealth(998);
	}

	if (UXGSimpleSubsystem* SelfSubstemPtr = UXGSimpleSubsystem::GetXGSubsystemMyself())
	{
		SelfSubstemPtr->AddHealth(500);
	}

}

// Called every frame
void AXGWorkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
