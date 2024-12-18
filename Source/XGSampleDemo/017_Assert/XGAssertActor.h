#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGAssertActor.generated.h"

UENUM(BlueprintType)
enum class EXGAssertType :uint8
{
	None,
	Left,
	Mid,
	Right,
	Max
};




UCLASS()
class XGSAMPLEDEMO_API AXGAssertActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGAssertActor();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void AttackEnemey(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void AttackEnemeyGood(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void VerifyMana();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void EnsuerVersion(bool bInVersion);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CheckNoEnry(EXGAssertType InXGAssertType);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CheckNoCode(bool bInAllRight);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CheckSlowf(bool bInAllRight);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ZeroBug(int32 InZero);

	UFUNCTION(BlueprintCallable, Category = "XG")
	void SetLocationWrong();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool ModifyHealth();

	int32 ModifyMana();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	int32 Health = 100;
	int32 Mana = 50;

};
