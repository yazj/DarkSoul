// Copyright Yazj, All Rights Reserved


#include "DarkSoulGameInstance.h"

UDarkSoulGameInstance::UDarkSoulGameInstance()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{
}

void UDarkSoulGameInstance::AddDefaultInventory(UDarkSoulSaveGame* SaveGame, bool bRemoveExtra)
{
}

bool UDarkSoulGameInstance::IsValidItemSlot(FDarkSoulItemSlot ItemSlot) const
{
	return false;
}

UDarkSoulSaveGame* UDarkSoulGameInstance::GetCurrentSaveGame()
{
	return nullptr;
}

void UDarkSoulGameInstance::SetSavingEnabled(bool bEnabled)
{
}

bool UDarkSoulGameInstance::LoadOrCreateSaveGame()
{
	return false;
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
