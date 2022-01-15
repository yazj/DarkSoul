// Fill out your copyright notice in the Description page of Project Settings.

#include "DarkSoulAssetManager.h"
#include "Items/DarkSoulItem.h"
#include "AbilitySystemGlobals.h"

UDarkSoulAssetManager& UDarkSoulAssetManager::Get()
{
	UDarkSoulAssetManager* This = Cast<UDarkSoulAssetManager>(GEngine->AssetManager);
	
	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogDarkSoul, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be DarkSoulAsstManager"));
		return *NewObject<UDarkSoulAssetManager>(); // never call this
	}
}

void UDarkSoulAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

UDarkSoulItem* UDarkSoulAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does an synchronous load and may hitch
	UDarkSoulItem* LoadedItem = Cast<UDarkSoulItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogDarkSoul, Warning, TEXT("Failed to load item for identifier %s"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
