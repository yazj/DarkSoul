//  Copyright Yazj, All Rights Reserved

#pragma once

#include "DarkSoul.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/DarkSoulAbilityTypes.h"
#include "DarkSoulGameplayAbility.generated.h"

/**
 *	Subclass of ability blueprint type with game-specific data
 *	This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 *	Most game will need to implement a subclass to support their game-specific code
 */
UCLASS()
class DARKSOUL_API UDarkSoulGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UDarkSoulGameplayAbility();

	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FDarkSoulGameplayEffectContainer> EffectContainerMap;

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FDarkSoulGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(
		const FDarkSoulGameplayEffectContainer& Container,
		const FGameplayEventData& EventData,
		int32 OverrideGameplayLevel = -1
	);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FDarkSoulGameplayEffectContainerSpec MakeEffectContainerSpec(
		FGameplayTag ContainerTag, 
		const FGameplayEventData& EventData, 
		int32 OverrideGameplayLevel = -1
	);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(
		const FDarkSoulGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(
		FGameplayTag ContainerTag, 
		const FGameplayEventData& EventData, 
		int32 OverrideGameplayLevel = -1
	);
	
};
