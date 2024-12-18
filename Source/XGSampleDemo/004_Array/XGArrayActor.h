#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGArrayActor.generated.h"

USTRUCT()
struct FXGAddStructInfo
{
	GENERATED_BODY()

public:

	FXGAddStructInfo()
	{
		UE_LOG(LogTemp, Warning, TEXT("我被初始化了0"));
	}

	FXGAddStructInfo(int32 InHealth)
		:Healty(InHealth)
	{
		UE_LOG(LogTemp, Warning, TEXT("我被初始化了1"));
	}


	~FXGAddStructInfo()
	{
		UE_LOG(LogTemp, Warning, TEXT("我被释放了"));
	}

	int32 Healty = 100;
};


USTRUCT()
struct FXGEqualStructInfo
{
	GENERATED_BODY()

public:

	FXGEqualStructInfo()
	{
		UE_LOG(LogTemp, Warning, TEXT("我被初始化了0"));
	}

	FXGEqualStructInfo(int32 InID)
		:ID(InID)
	{
		UE_LOG(LogTemp, Warning, TEXT("我被初始化了1"));
	}


	~FXGEqualStructInfo()
	{
		UE_LOG(LogTemp, Warning, TEXT("我被释放了"));
	}

	int32 ID = 0;


	bool operator==(const FXGEqualStructInfo& Other) const
	{

		return ID== Other.ID?true:false;
	}


};

USTRUCT()
struct FXGSortStructInfo
{
	GENERATED_BODY()

public:

	FXGSortStructInfo()
	{
	
	}

	FXGSortStructInfo(int32 InID, int32  InMoney)
		:ID(InID),
		Money(InMoney)
	{
	
	}


	~FXGSortStructInfo()
	{
	
	}

	int32 ID = 0;
	int32 Money = 0;

	bool operator<(const FXGSortStructInfo& Other) const
	{

		return ID < Other.ID ? true : false;
	}


};


USTRUCT()
struct FXGFindStructInfo
{
	GENERATED_BODY()

public:

	FXGFindStructInfo()
	{

	}

	FXGFindStructInfo(int32 InID, int32  InMoney)
		:ID(InID),
		Money(InMoney)
	{

	}


	~FXGFindStructInfo()
	{

	}

	int32 ID = 0;
	int32 Money = 0;

	inline bool operator==( const  int32& InID)
	{

		return ID == InID ;
	}


};

inline bool operator==(const  int32& InID,const FXGFindStructInfo& InStruct)
{

	return InStruct.ID == InID ;
}



UCLASS()
class XGSAMPLEDEMO_API AXGArrayActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXGArrayActor();

public:

	UFUNCTION(BlueprintCallable)
	void InitIntArray();

	UFUNCTION(BlueprintCallable)
	void AddStrArray();

	UFUNCTION(BlueprintCallable)
	void AddStructArray();

	UFUNCTION(BlueprintCallable)
	void EmplaceStructArray();

	UFUNCTION(BlueprintCallable)
	TArray<FString> AppendStrArray();

	UFUNCTION(BlueprintCallable)
	void AddUniqueString();

	UFUNCTION(BlueprintCallable)
	void InsertString();

	UFUNCTION(BlueprintCallable)
	void XGSetStringNum();

	UFUNCTION(BlueprintCallable)
	void AddUniqueStruct();

#pragma region ArrayRegion

	UFUNCTION(BlueprintCallable)
	void XGLoopArray1();

	UFUNCTION(BlueprintCallable)
	void XGLoopArray2();

	UFUNCTION(BlueprintCallable)
	void XGLoopArray3();

	UFUNCTION(BlueprintCallable)
	void XGLoopArray_Error();

	UFUNCTION(BlueprintCallable)
	void XGLoopArray_Right();

	UFUNCTION(BlueprintCallable)
	void XGLoopArray_Right_2();



#pragma endregion ArrayRegion

#pragma region Sort

	UFUNCTION(BlueprintCallable)
	void XGSortArray_Sort();

	UFUNCTION(BlueprintCallable)
	void XGSortArray_HeapSort();

	UFUNCTION(BlueprintCallable)
	void XGSortArray_StableSort();

	UFUNCTION(BlueprintCallable)
	void TestLambda();

	int32 CalculateMethod(int32 InNum, int32 InAddNum);

	UFUNCTION(BlueprintCallable)
	void XGSortArray_Sort_2();

	UFUNCTION(BlueprintCallable)
	void XGSortArray_HeapSort_2();

	UFUNCTION(BlueprintCallable)
	void XGSortArray_StableSort_2();


	UFUNCTION(BlueprintCallable)
	void XGSortStructArray_StableSort();




#pragma region Find


	UFUNCTION(BlueprintCallable)
	void XGFindArray();


	UFUNCTION(BlueprintCallable)
	void XGFindArray_Change();

	UFUNCTION(BlueprintCallable)
	void XGFindArray_Const();

	UFUNCTION(BlueprintCallable)
	void XGFindArray_ElementSize();

	UFUNCTION(BlueprintCallable)
	void XGGetAllocatedSize();

	
	UFUNCTION(BlueprintCallable)
	void XGIndexArray();


	UFUNCTION(BlueprintCallable)
	void XGIndexArray_IsValid();


	UFUNCTION(BlueprintCallable)
	void XGIndexArray_Upper();

	UFUNCTION(BlueprintCallable)
	void XGIndexArray_Latest();




#pragma endregion Find

#pragma region Contain

	UFUNCTION(BlueprintCallable)
	void XGContainArray();

	UFUNCTION(BlueprintCallable)
	void XGFindElementArray();


#pragma endregion Contain

	UFUNCTION(BlueprintCallable)
	void XGFindElementByKey();


	UFUNCTION(BlueprintCallable)
	void XGFindElementRetPtr();


	UFUNCTION(BlueprintCallable)
	void XGRemoveElement();



	UFUNCTION(BlueprintCallable)
	void XGRemoveMultiElement();


	UFUNCTION(BlueprintCallable)
	void XGOperateArray();


	UFUNCTION(BlueprintCallable)
	void XGOperateStrArray();

	UFUNCTION(BlueprintCallable)
	void XGHeapArray();

	UFUNCTION(BlueprintCallable)
	void XGSlackArray();

	UFUNCTION(BlueprintCallable)
	void XGEmptyArray();

	UFUNCTION(BlueprintCallable)
	void XGResetArray();

	UFUNCTION(BlueprintCallable)
	void XGAddCapArray();

	UFUNCTION(BlueprintCallable)
	void XGOriginArray();

	UFUNCTION(BlueprintCallable)
	void XGOriginArray2();


	UFUNCTION(BlueprintCallable)
	void XGZeroArray();


	UFUNCTION(BlueprintCallable)
	void XGZeroAndUninitArray();

	UFUNCTION(BlueprintCallable)
	void XGSwapArray();


	UFUNCTION(BlueprintCallable)
	void XGAddDefaultArray();

	UFUNCTION(BlueprintCallable)
	void XGRerversetArray();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FXGAddStructInfo> StructArray;


	int32 TempNum =100;
};
