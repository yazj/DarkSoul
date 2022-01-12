// Copyright Yazj, All Rights Reserved
#pragma once

#include "DarkSoul.h"
#include "DarkSoulInventoryInterface.h"
#include "GameFramework/PlayerController.h"
#include "DarkSoulPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API ADarkSoulPlayerController : public APlayerController, public IDarkSoulInventoryInterface
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	ADarkSoulPlayerController() {}
	virtual void BeginPlay() override;

	/** Map of all item owned by this player, from defined to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<UDarkSoulItem*, FDarkSoulItemData> InventoryData;

	/** Map of slot , from type/num to item, initailized from ItemSlotsPerType on DarkSoulGameInstance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FDarkSoulItemSlot, UDarkSoulItem*> SlottedItems;

	/** Delegate called when an inventory item has been added or removed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** Native version above, called before BP delegate */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** Called after the inventory was changed and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, UDarkSoulItem* Item);

	/** Called after an item was equipped and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void SlottedItemChanged(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item);

	/** Native version above, called before BP delegate */
	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	/** Delegate called when the inventory has been loaded/reloaded */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** Native varsion above , called before BP delegate */
	FOnInventoryLoadedNative OnInventoryLoadedNative;

	/** Adds a new inventory item, will add it to an empty slot if possible. If the item supports count you can add more than one count. It will also update the level when adding if required  */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(UDarkSoulItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	/** Remove an inventory item, will also remove from slots. A remove count of <= 0 means to remove all copies */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveInventoryItem(UDarkSoulItem* RemovedItem, int32 RemoveCount = 1);

	/** Return all inventory items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetInventoryItems(TArray<UDarkSoulItem*>& Items, FPrimaryAssetType ItemType);

	/** Returns number of instance of this item found in the inventory. This uses count from GetItemData */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetInventoryItemCount(UDarkSoulItem* Item) const;

	/** Returns the item data associated with an item, Returns false if none found */
	bool GetInventoryItemData(UDarkSoulItem* Item, FDarkSoulItemData& ItemData) const;

	/** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SetSlottedItem(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item);

	/** Return item in slot, or null if empty */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	UDarkSoulItem* GetSlottedItem(FDarkSoulItemSlot ItemSlot) const;

	/** Return all slotted items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetSlottedItems(TArray<UDarkSoulItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** Fills in any empty slots with items in inventory */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void FillEmptySlots();

	/** Manually save the inventory, this is called from add/remove functions automatically */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();

	// Implement IDarkSoulInventoryInterface
	virtual const TMap<UDarkSoulItem*, FDarkSoulItemData>& GetInventoryDataMap() const override
	{
		return InventoryData;
	}
	virtual const TMap<FDarkSoulItemSlot, UDarkSoulItem*>& GetSlottedItemMap() const override
	{
		return SlottedItems;
	}
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDalagate() override
	{
		return OnInventoryItemChangedNative;
	}
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChangedNative;
	}
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoadedNative;
	}

protected:
	/** Auto slots a specific item, return true if anything changed */
	bool FillEmptySlotWithItem(UDarkSoulItem* NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(bool bAdded, UDarkSoulItem* Item);
	void NotifySlottedItemChanged(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item);
	void NotifyInventoryLoaded();

	/** Called when a global save game has been loaded */
	void HandleSaveGameLoaded(UDarkSoulSaveGame* NewSaveGame);
	
};
