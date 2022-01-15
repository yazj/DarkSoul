// Copyright Yazj, All Rights Reserved

#include "DarkSoulPlayerController.h"
#include "Items/DarkSoulItem.h"
#include "DarkSoulCharacter.h"
#include "DarkSoulGameInstance.h"
#include "DarkSoulSaveGame.h"
#include "DarkSoulAssetManager.h"

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

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemSlotPerType)
	{
		for (int32 SlotNumber = 0; SlotNumber < Pair.Value; SlotNumber++)
		{
			SlottedItems.Add(FDarkSoulItemSlot(Pair.Key, SlotNumber), nullptr);
		}
	}

	UDarkSoulSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	UDarkSoulAssetManager& AssetManager = UDarkSoulAssetManager::Get();
	if (CurrentSaveGame)
	{
		// Copy from save game into controller data
		bool bFoundAnySlots = false;
		for (const TPair<FPrimaryAssetId, FDarkSoulItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
			UDarkSoulItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

			if (LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem, ItemPair.Value);
			}
		}

		for (const TPair<FDarkSoulItemSlot, FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if (SlotPair.Value.IsValid())
			{
				UDarkSoulItem* LoadedItem = AssetManager.ForceLoadItem(SlotPair.Value);
				if (GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{
					SlottedItems.Add(SlotPair.Key, LoadedItem);
					bFoundAnySlots = true;
				}
			}
		}

		if (!bFoundAnySlots)
		{
			// Auto slot items as no slots were saved
			FillEmptySlots();
		}

		NotifyInventoryLoaded();
		return true;

	}

	// Loaded failed but we reset inventory, so need to notify UI
	NotifyInventoryLoaded();

	return false;
}

bool ADarkSoulPlayerController::FillEmptySlotWithItem(UDarkSoulItem* NewItem)
{
	// Look for an empty item slot to fill with this item
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FDarkSoulItemSlot EmptySlot;

	for (TPair<FDarkSoulItemSlot, UDarkSoulItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				// Item already slotted
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				// We found an empty slot worth filling
				EmptySlot = Pair.Key;
			}
		}
	}

	if (EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot, NewItem);
		return true;
	}

	return false;

}

void ADarkSoulPlayerController::NotifyInventoryItemChanged(bool bAdded, UDarkSoulItem* Item)
{
	// Notify native before blueprint
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// Call BP update event
	InventoryItemChanged(bAdded, Item);
}

void ADarkSoulPlayerController::NotifySlottedItemChanged(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item)
{
	// Notify native before blueprint
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// Call BP update event 
	SlottedItemChanged(ItemSlot, Item);
}

void ADarkSoulPlayerController::NotifyInventoryLoaded()
{
	// Notify native before blueprint
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}

void ADarkSoulPlayerController::HandleSaveGameLoaded(UDarkSoulSaveGame* NewSaveGame)
{
	SaveInventory();
}

void ADarkSoulPlayerController::BeginPlay()
{
	// Load inventory off save game before starting play
	LoadInventory();

	Super::BeginPlay();
}