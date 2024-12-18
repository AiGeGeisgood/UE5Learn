// Copyright 2024 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XGBaseObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class XGSAMPLEDEMO_API UXGBaseObject : public UObject
{
	GENERATED_BODY()
	
public:

	UXGBaseObject();




	UXGBaseObject(const FObjectInitializer& ObjectInitializer);


	virtual ~UXGBaseObject();

};
