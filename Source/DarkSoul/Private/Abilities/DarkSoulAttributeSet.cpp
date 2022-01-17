// Copyright Yazj, All Rights Reversed

#include "Abilities/DarkSoulAttributeSet.h"
#include "Abilities/DarkSoulAbilitySystemComponent.h"
#include "DarkSoulCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UDarkSoulAttributeSet::UDarkSoulAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Mana(0.f)
	, MaxMana(0.f)
	, AttackPower(1.0f)
	, DefensePower(1.0f)
	, MoveSpeed(1.0f)
	, Damage(0.0f)
{}

void UDarkSoulAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UDarkSoulAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
}

void UDarkSoulAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDarkSoulAttributeSet, Health);
	DOREPLIFETIME(UDarkSoulAttributeSet, MaxHealth);
	DOREPLIFETIME(UDarkSoulAttributeSet, Mana);
	DOREPLIFETIME(UDarkSoulAttributeSet, MaxMana);
	DOREPLIFETIME(UDarkSoulAttributeSet, AttackPower);
	DOREPLIFETIME(UDarkSoulAttributeSet, DefensePower);
	DOREPLIFETIME(UDarkSoulAttributeSet, MoveSpeed);
}

void UDarkSoulAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{

}

void UDarkSoulAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, Health, OldValue);
}

void UDarkSoulAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, MaxHealth, OldValue);
}

void UDarkSoulAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, Mana, OldValue);
}

void UDarkSoulAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, MaxMana, OldValue);
}

void UDarkSoulAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, AttackPower, OldValue);
}

void UDarkSoulAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, DefensePower, OldValue);
}

void UDarkSoulAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDarkSoulAttributeSet, MoveSpeed, OldValue);
}
