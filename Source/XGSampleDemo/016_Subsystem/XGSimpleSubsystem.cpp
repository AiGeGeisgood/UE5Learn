#include "XGSimpleSubsystem.h"


UXGSimpleSubsystem* UXGSimpleSubsystem::MySubsystemPtr = nullptr;

bool UXGSimpleSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGSimpleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	MySubsystemPtr= this;


}

void UXGSimpleSubsystem::Deinitialize()
{

	MySubsystemPtr = nullptr;

	Super::Deinitialize();


}

void UXGSimpleSubsystem::Tick(float DeltaTime)
{

	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,TEXT("UXGSimpleSubsystemTick is executed"));


	if (bFirstTick)
	{
		bFirstTick =false;

		//执行初始化操作

	}

}

bool UXGSimpleSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId UXGSimpleSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGSimpleSubsystem, STATGROUP_Tickables);
}

void UXGSimpleSubsystem::AddHealth(int32 InHealthToAdd)
{
	CurrentHealth+= InHealthToAdd;
}

int32 UXGSimpleSubsystem::GetHealth()
{
	return CurrentHealth;
}

void UXGSimpleSubsystem::CallLocaion(FString InActionName, int32 InActionIndex)
{
	XGSubsystemActorLocationDelegate.Broadcast(InActionName, InActionIndex);

}

UXGSimpleSubsystem* UXGSimpleSubsystem::GetXGSubsystemMyself()
{
	return MySubsystemPtr;
}
