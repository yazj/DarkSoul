// Copyright XGSN, All Rights Reserved

#pragma once

// ----------------------------------------------------------------------------------------------------------------
// This header is for enums and structs used by classes and blueprints accross the game
// Collecting these in a single header helps avoid problems with recursive header includes
// It's also a good place to put things like data table row structs
// ----------------------------------------------------------------------------------------------------------------

#include "UObject/PrimaryAssetId.h"
#include "DarkSoulTypes.generated.h"

class UDarkSoulItem;
class UDarkSoulSaveGame;

/////////////////
// Item Slot

/** Struct representing a slot for an item, shown in the UI */
USTRUCT(BlueprintType)
struct DARKSOUL_API FDarkSoulItemSlot
{
	GENERATED_BODY()

	FDarkSoulItemSlot()
		: SlotNumber(-1) 
	{}

	FDarkSoulItemSlot(const FPrimaryAssetType& InItemType, int32 InSlotNumber)
		: ItemType(InItemType)
		, SlotNumber(InSlotNumber)
	{}

	/** The type of items that can go in this slot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FPrimaryAssetType ItemType;

	/** The number of this slot , 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotNumber;

	/** Equality operators */
	bool operator==(const FDarkSoulItemSlot& Other) const
	{
		return ItemType == Other.ItemType && SlotNumber == Other.SlotNumber;
	}
	bool operator!=(const FDarkSoulItemSlot& Other) const
	{
		return !(*this == Other);
	}

	/** Implemented so it can be used in Maps/Sets */
	friend inline uint32 GetTypeHash(const FDarkSoulItemSlot& Key)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
		Hash = HashCombine(Hash, (uint32)Key.SlotNumber);
		return Hash;
	}

	/** Returns ture if slot is valid*/
	bool IsValid() const
	{
		return ItemType.IsValid() && SlotNumber >= 0;
	}

};

////////////////////
// Item Data

/** Extra information about a UDarkSoulItem that is in a player's inventory */
USTRUCT(BlueprintType)
struct DARKSOUL_API FDarkSoulItemData
{
	GENERATED_BODY()

	/** Constructor, default to count/level 1 so declaring them in blueprint gives you the expection behavior */
	FDarkSoulItemData()
	: ItemCount(1)
	, ItemLevel(1)
	{}

	FDarkSoulItemData(int32 InItemCount, int32 InItemLevel)
		: ItemCount(InItemCount)
		, ItemLevel(InItemLevel)
	{}

	/** The number of instance of this item in the inventory, can never be below 1*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemCount;

	/** The level of this item, This level is shared for all instance, can never be below 1*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemLevel;

	/** Equality operators */
	bool operator==(const FDarkSoulItemData& Other) const
	{
		return ItemCount == Other.ItemCount && ItemLevel == Other.ItemLevel;
	}
	bool operator!=(const FDarkSoulItemData& Other) const
	{
		return !(*this == Other);
	}

	/** Return ture if count is greater than 0*/
	bool IsValid() const
	{
		return ItemCount > 0;
	}

	/** Append an item data, this adds the count and overrides everything else*/
	void UpdateItemData(const FDarkSoulItemData& Other, int32 MaxCount, int32 MaxLevel)
	{
		if (MaxCount <= 0)
		{
			MaxCount = MAX_int32;
		}

		if (MaxLevel <= 0)
		{
			MaxLevel = MAX_int32;
		}

		ItemCount = FMath::Clamp(ItemCount + Other.ItemCount, 1, MaxCount);
		ItemLevel = FMath::Clamp(Other.ItemLevel, 1, MaxLevel);

	}

};

/** Delegate called when an inventory item changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, bool, bAdded, UDarkSoulItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedNative, bool, UDarkSoulItem*);

/** Delegate called when the contents of an inventory slot changed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, FDarkSoulItemSlot, ItemSlot, UDarkSoulItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedNative, FDarkSoulItemSlot, UDarkSoulItem*);

/** Delegate called when the entire inventory has been loaded, all items may have been replaced */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoaded);
DECLARE_MULTICAST_DELEGATE(FOnInventoryLoadedNative);

/** Delegate called when the save game has been loaded/reset */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoaded, UDarkSoulSaveGame*, SaveGame);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoadedNative, UDarkSoulSaveGame*);



