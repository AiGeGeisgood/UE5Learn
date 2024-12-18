#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGTagType.h"
#include "GameplayTagAssetInterface.h"
#include "XGTagActor.generated.h"

UCLASS()
class XGSAMPLEDEMO_API AXGTagActor : public AActor,public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGTagActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	FGameplayTagContainer MyTagContainer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	FGameplayTagContainer MyStatusTagContainer;

};
