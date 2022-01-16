// Copyright Yazj, All Rights Reversed


#include "DarkSoulSaveGame.h"
#include "DarkSoulGameInstance.h"

void UDarkSoulSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != EDarkSoulGameVersion::LatestVersion)
	{
		// Convert from list item data map
		for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
		{
			InventoryData.Add(ItemId, FDarkSoulItemData(1, 1));
		}

		InventoryItems_DEPRECATED.Empty();

	}

	SavedDataVersion = EDarkSoulGameVersion::LatestVersion;

}
