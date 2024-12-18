#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "XGSimpleSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGSubsystemActorLocation,FString,InActionName,int32,InActionIndexTime);


UCLASS()
class XGSAMPLEDEMO_API UXGSimpleSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()


public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;




public:

	UFUNCTION(BlueprintCallable, Category = "XG")
	void AddHealth (int32 InHealthToAdd);

	UFUNCTION(BlueprintCallable, Category = "XG")
	int32 GetHealth();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void CallLocaion(FString InActionName,int32 InActionIndex);
	
	UFUNCTION(BlueprintCallable, Category = "XG")
	static UXGSimpleSubsystem* GetXGSubsystemMyself();


	UPROPERTY(BlueprintAssignable)
	FXGSubsystemActorLocation XGSubsystemActorLocationDelegate;

private:

	int32 CurrentHealth = 100;

	static UXGSimpleSubsystem* MySubsystemPtr;

	bool bFirstTick  =true;

};
