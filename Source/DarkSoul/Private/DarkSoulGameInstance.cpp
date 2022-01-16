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
		LoadedSave = Cast<UDarkSoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	return HandleSaveGameLoaded(LoadedSave);
}

bool UDarkSoulGameInstance::HandleSaveGameLoaded(USaveGame* SaveGameObject)
{
	bool bLoaded = false;

	if (!bSavingEnabled)
	{
		// If saving is disabled, ignore passed in object
		SaveGameObject = nullptr;
	}

	// Replace current save, old object will GC out
	CurrentSaveGame = Cast<UDarkSoulSaveGame>(SaveGameObject);

	if (CurrentSaveGame)
	{
		// Make sure it has newly added default inventory
		AddDefaultInventory(CurrentSaveGame, false);
		bLoaded = true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<UDarkSoulSaveGame>(UGameplayStatics::CreateSaveGameObject(UDarkSoulSaveGame::StaticClass()));

		AddDefaultInventory(CurrentSaveGame, true);
	}

	OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	OnSaveGameLoadedNative.Broadcast(CurrentSaveGame);

	return bLoaded;

}

void UDarkSoulGameInstance::GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const
{
	SlotName = SaveSlot;
	UserIndex = SaveUserIndex;
}

bool UDarkSoulGameInstance::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		if(bCurrentlySaving)
		{
			// Schedule another save to happen after current one finishes. We only queue one save
			bPendingSaveRequested = true;
			return true;
		}

		// Indicate that we're currently doing an async save
		bCurrentlySaving = true;

		// This goes off in the background
		UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex,
			FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UDarkSoulGameInstance::HandleAsyncSave));
		return true;
	}
	return false;
}

void UDarkSoulGameInstance::ResetSaveGame()
{
	// Call handle function with no loaded save, this will reset the data
	HandleSaveGameLoaded(nullptr);
}

void UDarkSoulGameInstance::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	ensure(bCurrentlySaving);
	bCurrentlySaving = false;

	if (bPendingSaveRequested)
	{
		// Start another save as we got a request while saving
		bPendingSaveRequested = false;
		WriteSaveGame();
	}
}
