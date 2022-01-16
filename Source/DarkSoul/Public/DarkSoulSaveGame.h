// Copyright Yazj, All Rights Reversed

#pragma once

#include "DarkSoul.h"
#include "Serialization/BulkData2.h"
#include "DarkSoulTypes.h"
#include "GameFramework/SaveGame.h"
#include "DarkSoulSaveGame.generated.h"

/**
* List of versions, native code will handle fixups for any old versions
*/
namespace EDarkSoulGameVersion
{
	enum type
	{
		// Initial version
		Initial,
		// Added Inventory
		AddedInventory,
		// Added ItemData to store count/level
		AddedItemData,

		// -------------<new version must be added before this line>----------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
}

/**
 * Object that is written to and read from the save game archive, with a data version
 */
UCLASS(BlueprintType)
class DARKSOUL_API UDarkSoulSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Constructor */
	UDarkSoulSaveGame()
	{
		// Set to current version, this will get overwritten during serialization when loading 
		SavedDataVersion = EDarkSoulGameVersion::LatestVersion;
	}

	/** Map of items to item data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FPrimaryAssetId, FDarkSoulItemData> InventoryData;

	/** Map of slotted items */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FDarkSoulItemSlot, FPrimaryAssetId> SlottedItems;

	/** User's unique id */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;

protected:
	/** Deprecated way of storing items, this is read in but not saved out */
	UPROPERTY()
	TArray<FPrimaryAssetId> InventoryItems_DEPRECATED;
	
	/** What LatestVersion was when the archive was saved */
	UPROPERTY()
	int32 SavedDataVersion;

	/** Overriden to allow version fixups */
	virtual void Serialize(FArchive& Ar) override;
};
