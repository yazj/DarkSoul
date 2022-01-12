// Copyright Yazj, All Rights Reserved

#pragma once

#include "DarkSoul.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"

#include "DarkSoulItem.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UDarkSoulItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Maximum number of instance that can be inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxCount;

	/** Maximum level of instance that can be inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxLevel;
	
};
