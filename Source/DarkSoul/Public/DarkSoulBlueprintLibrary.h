// Copyright Yazj, All Rights Reversed

#pragma once

#include "DarkSoul.h"
#include "DarkSoulTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DarkSoulBlueprintLibrary.generated.h"

/**
 * Game-specific blueprint library
 * Most games will need to implement one or more blueprint function libraries to expose their native code to blueprints
 */
UCLASS()
class DARKSOUL_API UDarkSoulBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//public:
//	/** Show the native loading screen, such as on a map transfer. If bPlayUntilStopped is false, 
//	*	it will be displayed for PlayTime and automatically stop
//	*/
//	UFUNCTION(BlueprintCallable, Category = Loading)
//	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);
//
//	/** Turns off the native loading screen if it is visible. This must be called if bPlayUntilStopped was true */
//	UFUNCTION(BlueprintCallable, Category = Loading)
//	static void StopLoadingScreen();
//
//	/** Returns true if this is being run from an editor preview */
//	UFUNCTION(BlueprintPure, Category = Loading)
//	static bool IsInEditor();
//
//	/** Equality operator for ItemSlot */
//	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (DarkSoulItemSlot)", CompactNodeTitle = "==", 
//		Keywords = "== equal"), Category = Inventory)
//	static bool Equal_DarkSoulItemSlot(const FDarkSoulItemSlot& A, const FDarkSoulItemSlot& B);
//
//	/** Inequality operator for ItemSlot */
//	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (DarkSoulItemSlot)", CompactNodeTitle = "!=",
//		Keywords = "!= not equal"), Category = Inventory)
//	static bool NotEqual_DarkSoulItemSlot(const FDarkSoulItemSlot& A, const FDarkSoulItemSlot& B);
//
//	/** Validity check for ItemSlot */
//	UFUNCTION(BlueprintPure, Category = Inventory)
//	static bool IsValidItemSlot(const FDarkSoulItemSlot& ItemSlot);
//
//	/** Checks if spec has any effects */
//	UFUNCTION(BlueprintPure, Category = Ability)
//	static bool DoesEffectContainerSpecHaveEffects(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec);
//
//	/** Checks if spec has any targets */
//	UFUNCTION(BlueprintPure, Category = Ability)
//	static bool DoesEffectContainerSpecHaveTargets(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec);
//
//	/** Adds target to a copy of the passed in effect container spec and returns it */
//	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "HitResults,TargetActors"))
//	static FDarkSoulGameplayEffectContainerSpec AddTargetsToEffectContainerSpec(
//		const FDarkSoulGameplayEffectContainerSpec& ContainerSpec,
//		const TArray<FHitResult>& HitResults,
//		const TArray<AActor*>& TargetActors);
//
//	/** Applies container spec that was made from an ability */
//	UFUNCTION(BlueprintCallable, Category = Ability)
//	static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec);
//
//	// Returns the project version set in the 'Project Settings' > 'Description' section of the editor'
//	UFUNCTION(BlueprintPure, Category = "Project")
//	static FString GetProjectVersion();
};
