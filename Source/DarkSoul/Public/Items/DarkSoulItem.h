// Copyright Yazj, All Rights Reserved

#pragma once

#include "DarkSoul.h"
#include "Engine/DataAsset.h"
#include "DarkSoulAssetManager.h"
#include "Styling/SlateBrush.h"
#include "Abilities/DarkSoulGameplayAbility.h"
#include "DarkSoulItem.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UDarkSoulItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Constructor */
	UDarkSoulItem()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
		, AbilityLevel(1)
	{}

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** User-Visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-Visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FSlateBrush ItemIcon;

	/** Price in game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Price;

	/** Maximum number of instance that can be inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxCount;

	/** Returns if the item is consumable, (MaxCount <= 0) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
	bool IsConsumable() const;

	/** Maximum level of instance that can be inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxLevel;

	/** Ability to grant if this item is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
	TSubclassOf<UDarkSoulGameplayAbility> GrantedAbility;
	
	/** Ability level this item grants, <= 0 means character level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
	int32 AbilityLevel;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overriden to use saved type  */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;


};
