#include "XGTagActor.h"


// UE_DEFINE_GAMEPLAY_TAG_STATIC(XG_Mode_Idle,"XG.Mode.Idle")

UE_DEFINE_GAMEPLAY_TAG_STATIC(XX_Mode_Idle, "XX.Mode.Idle")
// Sets default values
AXGTagActor::AXGTagActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AXGTagActor::BeginPlay()
{
	Super::BeginPlay();

	MyTagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("XG")));

	MyTagContainer.AddTag(XX_Mode_Idle);


}

void AXGTagActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(MyTagContainer);

	TagContainer.AppendTags(MyStatusTagContainer);

}



// Called every frame
void AXGTagActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
