
#pragma once

#include "CoreMinimal.h"
#include "XGWSMUtil.generated.h"


UCLASS()
class UXGWSMUtilBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	static	TArray<uint8> XGMessageEncode(const FString& InMessage);

	static	FString XGMessageDecode(TArray<uint8>& InData);

	static	FString XGMessageDecode(const  uint8* InDataPtr, int32 InDataSize);


};
