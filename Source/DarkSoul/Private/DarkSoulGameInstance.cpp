// Copyright Yazj, All Rights Reserved


#include "DarkSoulGameInstance.h"
#include "DarkSoulSaveGame.h"
#include "DarkSoulTypes.h"
#include "DarkSoulAssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Items/DarkSoulItem.h"

UDarkSoulGameInstance::UDarkSoulGameInstance()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{
}

void UDarkSoulGameInstance::AddDefaultInventory(UDarkSoulSaveGame* SaveGame, bool bRemoveExtra)
{
	// if we want to remove extra, clear out the existing inventory
	if (bRemoveExtra)
	{
		SaveGame->InventoryData.Reset();
	}

	// if we want to add default inventory, this only adds if not already in the inventory
	for (const TPair<FPrimaryAssetId, FDarkSoulItemData>& Pair : DefaultInventory)
	{
		if (!SaveGame->InventoryData.Contains(Pair.Key))
		{
			SaveGame->InventoryData.Add(Pair.Key, Pair.Value);
		}
	}
}

bool UDarkSoulGameInstance::IsValidItemSlot(FDarkSoulItemSlot ItemSlot) const
{
	if (ItemSlot.IsValid())
	{
		const int32* FoundCount = ItemSlotPerType.Find(ItemSlot.ItemType);

		if (FoundCount)
		{
			return ItemSlot.SlotNumber < *FoundCount;
		}
	}
	return false;
}

UDarkSoulSaveGame* UDarkSoulGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void UDarkSoulGameInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = true;
}

bool UDarkSoulGameInstance::LoadOrCreateSaveGame()
{
	UDarkSoulSaveGame* LoadedSave = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{

	}
}

bool UDarkSoulGameInstance::HandleSaveGameLoaded(USaveGame* SaveGameObject)
{
	return false;
}

void UDarkSoulGameInstance::GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const
{
}

bool UDarkSoulGameInstance::WriteSaveGame()
{
	return false;
}

void UDarkSoulGameInstance::ResetSaveGame()
{
}

void UDarkSoulGameInstance::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
}
