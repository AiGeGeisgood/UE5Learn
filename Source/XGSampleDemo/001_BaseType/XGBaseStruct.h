
#pragma once

#include "CoreMinimal.h"
#include "XGBaseStruct.generated.h"

USTRUCT(BlueprintType)
struct FXGBaseStruct
{
	GENERATED_USTRUCT_BODY();

public:

	UPROPERTY()
	FString StructName =TEXT("");

};

USTRUCT(BlueprintType)
struct FXGPropertyStruct2
{
	GENERATED_USTRUCT_BODY();

public:

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	FString StructName1 = TEXT("");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString StructName2 = TEXT("");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* ObjectPtr = nullptr;


};

UCLASS(BlueprintType)
class XGSAMPLEDEMO_API UXGBaseObject2 : public UObject
{
	GENERATED_BODY()

public:

	UXGBaseObject2();




	UXGBaseObject2(const FObjectInitializer& ObjectInitializer);


	virtual ~UXGBaseObject2();

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FXGPropertyStruct2 MMStruct;

};
