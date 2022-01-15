//  Copyright Yazj, All Rights Reserved

#pragma once

#include "DarkSoul.h"
#include "DarkSoulTypes.h"
#include "Engine/GameInstance.h"
#include "DarkSoulGameInstance.generated.h"

class UDarkSoulItem;
class UDarkSoulSaveGame;

/**
 *	Base class of GameInstance, should be blueprinted
 *  Most game will need to make a game-specific subclass of GameInstance
 *	Once you make a blueprint subclass of your native subclass you will to set it to be the default in project settings
 */
UCLASS()
class DARKSOUL_API UDarkSoulGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Constructor
	UDarkSoulGameInstance();

	/** List of inventpry items to add to new players */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FPrimaryAssetId, FDarkSoulItemData> DefaultInventory;

	/** Number of slots for each type of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FPrimaryAssetType, int32> ItemSlotPerType;

	/** The slot name for saving */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SaveSlot;

	/** The platform-specific user index */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	/** Delegate called when the save game has been loaded/reset */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSaveGameLoaded OnSaveGameLoaded;

	/** Native delegate for save game load/reset */
	FOnSaveGameLoadedNative OnSaveGameLoadedNative;

	/**
	*	Adds the default inventory to the inventory array
	*	@param InventoryArray Inventory to modify
	*	@param RemoveExtra If true, remove anything other than default inventory
	*/
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddDefaultInventory(UDarkSoulSaveGame* SaveGame, bool bRemoveExtra = false);

	/** Returns true if this is a valid inventory slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsValidItemSlot(FDarkSoulItemSlot ItemSlot) const;

	/** Returns the current save game, so it can be used to initialize stat. Changes are not written untill WriteSaveGame is called  */
	UFUNCTION(BlueprintCallable, Category = Save)
	UDarkSoulSaveGame* GetCurrentSaveGame();

	/** Sets rather save/load is enabled. If disabled it will always count as a new character */
	UFUNCTION(BlueprintCallable, Category = Save)
	void SetSavingEnabled(bool bEnabled);

	/** 
	*	Synchronously loads a save game. If it fails, it will create a new one.
	*	Returns true if it loaded, false if it created one
	*/
	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateSaveGame();

	/** Handle the final setup required after loading a USaveGame object using AsyncLoadGameFromSlot.
	*	Returns true if it loaded, false if it created one
	*/
	UFUNCTION(BlueprintCallable, Category = Save)
	bool HandleSaveGameLoaded(USaveGame* SaveGameObject);

	/** Gets the save game slot and user index used for inventory saving, 
	ready to pass to GameplayStatics save functions  */
	UFUNCTION(BlueprintCallable, Category = Save)
	void GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const;

	/** Writes the current save game object to disk, The save to disk happens in a background thread */
	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	/** 
	*  Resets the current save game to it's default. 
	*  This will erase player data! 
	*  This won't save to disk until the next WriteSaveGame.
	*/
	UFUNCTION(BlueprintCallable, Category = Save)
	void ResetSaveGame();

protected:
	/** The current save game object */
	UPROPERTY()
	UDarkSoulSaveGame* CurrentSaveGame;

	/** Rather it will attempt to actually save to disk */
	UPROPERTY()
	bool bSavingEnabled;

	/** True if we are in the middle of doing a save */
	UPROPERTY()
	bool bCurrentlySaving;

	/** True if another save was requested during a save */
	UPROPERTY()
	bool bPendingSaveRequested;

	/** Called when the async save happens */
	virtual void HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
