#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGINIActor.generated.h"
//https://mp.weixin.qq.com/s?__biz=MzAxNzMzODkyMA==&mid=2650679499&idx=1&sn=354439a6e8869b3ffd246aaac1888ca9&chksm=83ed8f8cb49a069ab3e8bfedd7e231bbd14f4772869a9c420fc30f70665659da6d5b3955e836&scene=178&cur_album_id=1883930424033280000#rd
UCLASS(config = Game)
class XGSAMPLEDEMO_API AXGINIActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGINIActor();

	UPROPERTY(Config)
	int32 MyConfigVariable;


	UFUNCTION(BlueprintCallable, Category = "XG")
	void GetINIVariable();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void ModifyConfigVariable();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
