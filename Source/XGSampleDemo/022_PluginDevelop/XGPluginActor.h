#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGPluginActor.generated.h"


/**
* 商城插件指南
* https://www.unrealengine.com/zh-CN/marketplace-guidelines
*
*
*/

/**
* 访问后生成的链接:
* https://drive.google.com/file/d/1jXlu8FZRuIYKXWcBxHvUSsfZorPzcngl/view
* 手动拼写成直链接:
* https://drive.google.com/uc?export=download&id=1jXlu8FZRuIYKXWcBxHvUSsfZorPzcngl
* 
*/

UCLASS()
class XGSAMPLEDEMO_API AXGPluginActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGPluginActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
