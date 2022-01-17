// Copyright Yazj , All Rights Reserve

#pragma once

/*
*	This header is for Ability-specific structures and enums that are shared across a project
*	Every game will probably need a file like this to handle their extensions to the system
*	This file is a good place for subclasses of FGameplayEffectContex and FGameplayAbilityTargetData
*/

#include "DarkSoul.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "DarkSoulAbilityTypes.generated.h"

class UDarkSoulAbilitySystemComponent;
class UGameplayEffect;
class UDarkSoulTargetType;

/**
*	Struct defining a list of gameplay effects, a tag , and targeting info
*	These containers are defined statically in blueprints or assets and then turn into Specs at runtime
*/
USTRUCT(BlueprintType)
struct FDarkSoulGameplayEffectContainer
{
	GENERATED_BODY()
public:
	FDarkSoulGameplayEffectContainer() {}

	/** Sets the way that targeting happens */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameplayEffectContainer)
	TSubclassOf<UDarkSoulTargetType> TargetType;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;

};

/**
*	A "processed" version of DarkSoulGameplayEffectContainer that can be passed around and eventually applied
*/
USTRUCT(BlueprintType)
struct FDarkSoulGameplayEffectContainerSpec
{
	GENERATED_BODY()
public:

	FDarkSoulGameplayEffectContainerSpec() {}

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameplayEffectContainer)
	TArray<FGameplayAbilityTargetDataHandle> TargetGameplayEffectSpecs;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const;

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const;

	/** Adds new targets to target data */
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);


};