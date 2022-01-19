// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DarkSoulItem.h"
#include "MyDarkSoulPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UMyDarkSoulPotionItem : public UDarkSoulItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	UMyDarkSoulPotionItem()
	{
		//ItemType = UDarkSoulAssetManager::PotionItemType;
	}
	
};
