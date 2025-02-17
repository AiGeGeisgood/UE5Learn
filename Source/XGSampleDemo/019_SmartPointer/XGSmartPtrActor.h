#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGSmartPtrActor.generated.h"


class FRegistryObject;
class FMyBaseClass;

class FRegistryObject
{
	// 此函数将接受到FMyBaseClass或其子类的TSharedRef或TSharedPtr。
public:
	void Register(TSharedRef<FMyBaseClass>)
	{
	}
};


class FMyBaseClass : public TSharedFromThis<FMyBaseClass>
{
public:
	FMyBaseClass()
	{
	}

	virtual ~FMyBaseClass()
	{
	}

	virtual void Register(FRegistryObject* RegistryObject)

	{
		// 访问对"this"的共享引用。

		// 直接继承自< TSharedFromThis >，因此AsShared()和SharedThis(this)会返回相同的类型。

		TSharedRef<FMyBaseClass> ThisAsSharedRef = AsShared();

		// RegistryObject需要 TSharedRef<FMyBaseClass>，或TSharedPtr<FMyBaseClass>。TSharedRef可被隐式转换为TSharedPtr.

		RegistryObject->Register(ThisAsSharedRef);
	}
};

class FMyDerivedClass : public FMyBaseClass
{
public:
	FMyDerivedClass()
	{
	}

	virtual ~FMyDerivedClass()
	{
	}

	virtual void Register(FRegistryObject* RegistryObject) override

	{
		// 并非直接继承自TSharedFromThis<>，因此AsShared()和SharedThis(this)不会返回相同类型。

		// 在本例中，AsShared()会返回在TSharedFromThis<> - TSharedRef<FMyBaseClass>中初始指定的类型。

		// 在本例中，SharedThis(this)会返回具备"this"类型的TSharedRef - TSharedRef<FMyDerivedClass>。

		// SharedThis()函数仅在与 'this'指针相同的范围内可用。

		TSharedRef<FMyDerivedClass> AsSharedRef = SharedThis(this);

		// FMyDerivedClass是FMyBaseClass的一种类型，因此RegistryObject将接受TSharedRef<FMyDerivedClass>。

		RegistryObject->Register(AsSharedRef);
	}
};


//USTRUCT(BlueprintType)
struct FSmartPtrStruct : public TSharedFromThis<FSmartPtrStruct>
{
	//GENERATED_BODY()

	FSmartPtrStruct(int32 InAABB)
		: aa(InAABB)
	{
		UE_LOG(LogTemp, Warning, TEXT("FSmartPtrStruct构造了aa=%d"), aa);
	}


	FSmartPtrStruct()
	{
		UE_LOG(LogTemp, Warning, TEXT("FSmartPtrStruct构造了"));
	}

	~FSmartPtrStruct()
	{
		UE_LOG(LogTemp, Warning, TEXT("FSmartPtrStruct析构了 aa=%d"), aa);
	}


	void PrintAA()
	{
		UE_LOG(LogTemp, Warning, TEXT("PrintAA aa=%d"), aa);
	}

	void MyThisShow()
	{
		auto a = SharedThis(this);
	}

	int32 aa = -1;

	//错误的写法
	TSharedPtr<FSmartPtrStruct> MyHoldPtr = nullptr;

	//正确的写法
	TWeakPtr<FSmartPtrStruct> MyHoldWeakPtr = nullptr;
};


UCLASS()
class XGSAMPLEDEMO_API AXGSmartPtrActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGSmartPtrActor();


	void InitMySPStruct();

	UFUNCTION(BlueprintCallable)
	void InitSPStruct();

	UFUNCTION(BlueprintCallable)
	void ResetSPStruct();

	UFUNCTION(BlueprintCallable)
	void MoveSPStruct();

	UFUNCTION(BlueprintCallable)
	void ConvertSPStruct();

	UFUNCTION(BlueprintCallable)
	void EQSPStruct();

	UFUNCTION(BlueprintCallable)
	void DeleteStruct();

	UFUNCTION(BlueprintCallable)
	void MyRef();

	UFUNCTION(BlueprintCallable)
	void MyWeakPtr();

	UFUNCTION(BlueprintCallable)
	void MyWeakPtr2();

	UFUNCTION(BlueprintCallable)
	void MyLoopPtr();

	UFUNCTION(BlueprintCallable)
	void MyLoopPtr2();

	UFUNCTION(BlueprintCallable)
	void MyUniquePtr();

	UFUNCTION(BlueprintCallable)
	void MyAsShared();

	UFUNCTION(BlueprintCallable)
	void MySSharedThis();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
