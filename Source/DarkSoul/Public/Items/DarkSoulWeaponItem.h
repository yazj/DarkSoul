// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoul.h"
#include "DarkSoulAssetManager.h"
#include "Items/DarkSoulItem.h"
#include "DarkSoulWeaponItem.generated.h"

/**
 * Native base class for weapon, should be blueprinted
 */
UCLASS()
class DARKSOUL_API UDarkSoulWeaponItem : public UDarkSoulItem
{
	GENERATED_BODY()
	
public:
	UDarkSoulWeaponItem()
	{
		//ItemType = UDarkSoulAssetManager::WeaponItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> WeaponActor;
};
