// Copyright Yazj, All Rights Reserved

#include "DarkSoulPlayerController.h"
#include "Items/DarkSoulItem.h"
#include "DarkSoulCharacter.h"
#include "DarkSoulGameInstance.h"
#include "DarkSoulSaveGame.h"

bool ADarkSoulPlayerController::AddInventoryItem(UDarkSoulItem* NewItem, int32 ItemCount, int32 ItemLevel, bool bAutoSlot)
{
	bool bChanged = false;
	if (!NewItem)
	{
		UE_LOG(LogDarkSoul, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
		return false;
	}

	if (ItemCount <= 0 || ItemLevel <= 0)
	{
		UE_LOG(LogDarkSoul, Warning,
			TEXT("AddInventoryItem: Failed trying to add item %s with negative count or level"), 
			*NewItem->GetName());
		return false;
	}

	// Find current item data, which may be empty
	FDarkSoulItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	// Find modifitied data
	FDarkSoulItemData NewData = OldData;
	NewData.UpdateItemData(FDarkSoulItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		// If data changed, need to update storage and call callback
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		// Slot item if required
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		// if anything changed, write to save game
		SaveInventory();
		return true;
	}

	return false;

}

bool ADarkSoulPlayerController::RemoveInventoryItem(UDarkSoulItem* RemovedItem, int32 RemoveCount)
{



	return false;
}

void ADarkSoulPlayerController::GetInventoryItems(TArray<UDarkSoulItem*>& Items, FPrimaryAssetType ItemType)
{
	return;
}

int32 ADarkSoulPlayerController::GetInventoryItemCount(UDarkSoulItem* Item) const
{
	return int32();
}

bool ADarkSoulPlayerController::GetInventoryItemData(UDarkSoulItem* Item, FDarkSoulItemData& ItemData) const
{
	return false;
}

bool ADarkSoulPlayerController::SetSlottedItem(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item)
{
	return false;
}

UDarkSoulItem* ADarkSoulPlayerController::GetSlottedItem(FDarkSoulItemSlot ItemSlot) const
{
	return nullptr;
}

void ADarkSoulPlayerController::GetSlottedItems(TArray<UDarkSoulItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	return;
}

void ADarkSoulPlayerController::FillEmptySlots()
{
	return;
}

bool ADarkSoulPlayerController::SaveInventory()
{
	return false;
}

bool ADarkSoulPlayerController::LoadInventory()
{
	return false;
}

bool ADarkSoulPlayerController::FillEmptySlotWithItem(UDarkSoulItem* NewItem)
{
	return false;
}

void ADarkSoulPlayerController::NotifyInventoryItemChanged(bool bAdded, UDarkSoulItem* Item)
{
	return;
}

void ADarkSoulPlayerController::NotifySlottedItemChanged(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item)
{
	return;
}

void ADarkSoulPlayerController::NotifyInventoryLoaded()
{
	return;
}

void ADarkSoulPlayerController::HandleSaveGameLoaded(UDarkSoulSaveGame* NewSaveGame)
{
	return;
}

void ADarkSoulPlayerController::BeginPlay()
{
	// Load inventory off save game before starting play
	LoadInventory();

	Super::BeginPlay();
}