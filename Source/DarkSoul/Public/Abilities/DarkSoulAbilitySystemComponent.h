// Copyright Yazj , All Rights Reserve

#pragma once

#include "DarkSoul.h"
#include "AbilitySystemComponent.h"
#include "Abilities/DarkSoulGameplayAbility.h"
#include "Abilities/DarkSoulAbilityTypes.h"
#include "DarkSoulAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UDarkSoulAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	UDarkSoulAbilitySystemComponent() {}

	/** Returns a list of currently active ability instance that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UDarkSoulGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UDarkSoulAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	
	
};
