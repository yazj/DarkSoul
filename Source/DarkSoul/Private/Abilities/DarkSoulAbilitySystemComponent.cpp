// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/DarkSoulAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/DarkSoulGameplayAbility.h"
#include "DarkSoulCharacter.h"

void UDarkSoulAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UDarkSoulGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UDarkSoulGameplayAbility>(ActiveAbility));
		}
	}
}

int32 UDarkSoulAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	ADarkSoulCharacter* OwningCharacter = Cast<ADarkSoulCharacter>(GetOwnerActor());

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

UDarkSoulAbilitySystemComponent* UDarkSoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UDarkSoulAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
