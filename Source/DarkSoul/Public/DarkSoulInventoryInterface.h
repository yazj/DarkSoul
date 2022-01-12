// Copyright Yazj, All Rights Reserved

#pragma once

#include "DarkSoul.h"
#include "DarkSoulTypes.h"
#include "DarkSoulInventoryInterface.generated.h"

/**
*	Interface for actors that provide a set of DarkSoulItem bound to ItemSlots
*	This exists so DarkSoulCharacter can query inventory without doing hacky player controller casts
*	It is designed only for use by native classes
*/
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBluprint))
class UDarkSoulInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class DARKSOUL_API IDarkSoulInventoryInterface
{
	GENERATED_BODY()

public:
	/** Returns the map of items to data*/
	virtual const TMap<UDarkSoulItem*, FDarkSoulItemData>& GetInventoryDataMap() const = 0;

	/** Returns the map of slots to items */
	virtual const TMap<FDarkSoulItemSlot, UDarkSoulItem*>& GetSlottedItemMap() const = 0;

	/** Gets the delegate for inventory item changes */
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDalagate() = 0;

	/** Gets the delegate for inventory slot changes */
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() = 0;

	/** Gets the delegate for when the inventory loads */
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() = 0;

};