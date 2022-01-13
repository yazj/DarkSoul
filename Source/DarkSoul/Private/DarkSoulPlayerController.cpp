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
	if (!RemovedItem)
	{
		UE_LOG(LogDarkSoul, Warning, TEXT("RemoveInventoryItem: Failed trying to remove null item!"));
		return false;
	}

	// Find current item data, which may be empty
	FDarkSoulItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		// Wasn't found
		return false;
	}

	// If RemoveCount <= 0, delete all
	if (RemoveCount <= 0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if (NewData.ItemCount > 0)
	{
		// Update data with new count
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		// Removed item entirely, make sure it is unslotted
		InventoryData.Remove(RemovedItem);

		for (TPair<FDarkSoulItemSlot, UDarkSoulItem*>& Pair : SlottedItems)
		{
			if (Pair.Value == RemovedItem)
			{
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key, Pair.Value);
			}
		}
	}

	// If we got this far, therer is a change so notify and save
	NotifyInventoryItemChanged(false, RemovedItem);

	SaveInventory();
	return false;
}

void ADarkSoulPlayerController::GetInventoryItems(TArray<UDarkSoulItem*>& Items, FPrimaryAssetType ItemType)
{
	for (const TPair<UDarkSoulItem*, FDarkSoulItemData>& Pair : InventoryData)
	{
		if (Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			// Filters based on item type
			if (AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
				Items.Add(Pair.Key);
			}
		}
	}
}

int32 ADarkSoulPlayerController::GetInventoryItemCount(UDarkSoulItem* Item) const
{
	const FDarkSoulItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		return FoundItem->ItemCount;
	}
	return 0;

}

bool ADarkSoulPlayerController::GetInventoryItemData(UDarkSoulItem* Item, FDarkSoulItemData& ItemData) const
{
	const FDarkSoulItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		ItemData = *FoundItem;
		return true;
	}
	ItemData = FDarkSoulItemData(0, 0);
	return false;
}

bool ADarkSoulPlayerController::SetSlottedItem(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item)
{
	// Interate entire inventory because we need to remove from old slot
	bool bFound = false;
	for (TPair<FDarkSoulItemSlot, UDarkSoulItem*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			// Add to new slot 
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// If this item was found in another slot, remove it
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}

	return false;
}

UDarkSoulItem* ADarkSoulPlayerController::GetSlottedItem(FDarkSoulItemSlot ItemSlot) const
{
	UDarkSoulItem* const* FoundItem = SlottedItems.Find(ItemSlot);

	if (FoundItem)
	{
		return *FoundItem;
	}
	return nullptr;
}

void ADarkSoulPlayerController::GetSlottedItems(TArray<UDarkSoulItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	for (TPair<FDarkSoulItemSlot, UDarkSoulItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

void ADarkSoulPlayerController::FillEmptySlots()
{
	bool bShouldSave = false;
	for (const TPair<UDarkSoulItem*, FDarkSoulItemData>& Pair : InventoryData)
	{
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}

	if (bShouldSave)
	{
		SaveInventory();
	}
}

bool ADarkSoulPlayerController::SaveInventory()
{
	UWorld* World = GetWorld();
	UDarkSoulGameInstance* GameInstance = World ? World->GetGameInstance<UDarkSoulGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UDarkSoulSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// Reset cached data in save game before writing to it
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for (const TPair<UDarkSoulItem*, FDarkSoulItemData>& ItemPair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if (ItemPair.Key)
			{
				AssetId = ItemPair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, ItemPair.Value);
			}
		}

		for (const TPair<FDarkSoulItemSlot, UDarkSoulItem*>& SlotPair : SlottedItems)
		{
			FPrimaryAssetId AssetId;

			if (SlotPair.Value)
			{
				AssetId = SlotPair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(SlotPair.Key, AssetId);
		}

		// Now that cache is updated, write to disk
		GameInstance->WriteSaveGame();
		return true;
	}

	return false;
}

bool ADarkSoulPlayerController::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();

	// Fill in slots from game instance
	UWorld* World = GetWorld();
	UDarkSoulGameInstance* GameInstance = World ? World->GetGameInstance<UDarkSoulGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	// Bind to loaded callback if not already bound
	if (!GameInstance->OnSaveGameLoadedNative.IsBoundToObject(this))
	{
		GameInstance->OnSaveGameLoadedNative.AddUObject(this, &ADarkSoulPlayerController::HandleSaveGameLoaded);
	}

	// ......

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