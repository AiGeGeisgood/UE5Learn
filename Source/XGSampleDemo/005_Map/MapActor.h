#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapActor.generated.h"

USTRUCT()
struct FXGMapInfo
{

	GENERATED_BODY()


	int32 Health=-1;

	FString NPCName =TEXT("None");

	FXGMapInfo()
	{

	}

	FXGMapInfo(FString InNPCName)
	:NPCName(InNPCName)
	{

	}


	bool operator==(const FXGMapInfo& Other) const
	{

		return NPCName == Other.NPCName ? true : false;
	}


	friend uint32 GetTypeHash(const FXGMapInfo& Other)
	{
		return GetTypeHash(Other.NPCName);
	}

};

struct FMyStruct
{
	// String which identifies our key
	FString UniqueID =TEXT("");

	// Some state which doesn't affect struct identity
	float SomeFloat  =0.f;


	explicit FMyStruct(float InFloat)
		: UniqueID(FGuid::NewGuid().ToString())
		, SomeFloat(InFloat)
	{

	}

};

template <typename ValueType>
struct TMyStructMapKeyFuncs : BaseKeyFuncs<	TPair<FMyStruct, ValueType>, FString>
{

private:

	typedef BaseKeyFuncs<TPair<FMyStruct, ValueType>,FString> Super;

public:

	typedef typename Super::ElementInitType ElementInitType;
	typedef typename Super::KeyInitType     KeyInitType;


	static KeyInitType GetSetKey(ElementInitType Element)
	{
		return Element.Key.UniqueID;
	}

	static bool Matches(KeyInitType A, KeyInitType B)
	{
		return A.Compare(B, ESearchCase::CaseSensitive) == 0;
	}

	static uint32 GetKeyHash(KeyInitType Key)
	{
		return FCrc::StrCrc32(*Key);
	}

};








UCLASS()
class XGSAMPLEDEMO_API AMapActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMapActor();


public:

	UFUNCTION(BlueprintCallable)
	void InitMap();

	UFUNCTION(BlueprintCallable)
	void IterateMap();

	UFUNCTION(BlueprintCallable)
	void QueryMap();

	UFUNCTION(BlueprintCallable)
	void FindMap();

	UFUNCTION(BlueprintCallable)
	void FindAdvMap();

	UFUNCTION(BlueprintCallable)
	void FindKeyMap();

	UFUNCTION(BlueprintCallable)
	void XGGetAllKeysAndValueMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void XGRemoveMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void XGRemoveCheckMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void RemoveAndCopyValueMap();
	

	UFUNCTION(BlueprintCallable, Category = "XG")
	void EmptyMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ResetMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void ReverseMap();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void SortMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void OperateMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void SlackMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void StructMap();


	UFUNCTION(BlueprintCallable, Category = "XG")
	void StructKeyFunMap();

	UFUNCTION(BlueprintCallable, Category = "XG")
	void StructSize();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "XG|Fruit")
	TMap<int32, FString> MyFruitMap;


};
