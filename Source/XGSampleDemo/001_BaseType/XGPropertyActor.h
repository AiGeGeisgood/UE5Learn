#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XGPropertyActor.generated.h"


UENUM(Meta = (Bitflags))

enum class EColorBits1

{

	ECB_Red,

	ECB_Green,

	ECB_Blue

};



UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))

enum class EColorBits2

{

	ECB_Red= 0x01,

	ECB_Green= 0x02,

	ECB_Blue= 0x04

};


USTRUCT(BlueprintType)
struct FXGPropertyStruct
{
	GENERATED_USTRUCT_BODY();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	TArray<FString> OneStrArray;;

};



UCLASS()
class XGSAMPLEDEMO_API AXGPropertyActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AXGPropertyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	uint8 MyUInt8 = 0;

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	uint16 MyUInt16 = 0;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	uint32 MyUInt32 = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	int64 MyUInt64 = 0;

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	int8 MyInt8 = 0;

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	int16 MyInt16 = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	int32 MyInt32 = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	int64 MyInt64 = 0;


	/*~ BasicBits appears as a list of generic flags in the editor, instead of an integer field. */

	UPROPERTY(EditAnywhere, Meta = (Bitmask))

	int32 BasicBits;



	/*~ You can set MyFunction using a generic list of flags instead of typing in an integer value. */

	UFUNCTION(BlueprintCallable)

	void MyFunction(UPARAM(meta = (Bitmask)) int32 BasicBitsParam);



	/*~ This property lists flags matching the names of values from EColorBits. */

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EColorBits1"))

	int32 ColorFlags1;

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EColorBits2"))

	int32 ColorFlags2;


	/*~ MyOtherFunction shows flags named after the values from EColorBits. */

	UFUNCTION(BlueprintCallable)

	void MyOtherFunction(UPARAM(meta = (Bitmask, BitmaskEnum = "EColorBits2")) int32 ColorFlagsParam);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	uint32 bIsHungry : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	bool bIsThirsty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	float MyFloat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	double MyDouble;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	FString MyString =TEXT("我是字符串String");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	FText MyText = NSLOCTEXT("MyActor","MyText","我是字符串Name");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	FName MyName =TEXT("我是字符串Name");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	bool bEdite =false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG", meta = (
		EditCondition = "bEdite"))
	FString EditString = TEXT("我是 是否可以编辑字符串String");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	TArray<FString> MyStrArray;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	TArray < TArray<FString>> MyStrArray1;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XG")
	TArray < FXGPropertyStruct> TwoTwoStrArray;
};