// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoul.h"
#include "UObject/NoExportTypes.h"
#include "Abilities/DarkSoulAbilityTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "DarkSoulTargetType.generated.h"

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class DARKSOUL_API UDarkSoulTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UDarkSoulTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(
		ADarkSoulCharacter* TargetingCharacter,
		AActor* TargetingActor,
		FGameplayEventData EventData,
		TArray<FHitResult>& OutHitResults,
		TArray<AActor*>& OutActors
	) const;
	
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class DARKSOUL_API UDarkSoulTargteType_UseOwner : public UDarkSoulTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UDarkSoulTargteType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(
		ADarkSoulCharacter* TargetingCharacter,
		AActor* TargetingActor,
		FGameplayEventData EventData,
		TArray<FHitResult>& OutHitResults,
		TArray<AActor*>& OutActors
	) const override;

};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class DARKSOUL_API UDarkSoulTargteType_UseEventData : public UDarkSoulTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UDarkSoulTargteType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(
		ADarkSoulCharacter* TargetingCharacter, 
		AActor* TargetingActor, 
		FGameplayEventData EventData, 
		TArray<FHitResult>& OutHitResults, 
		TArray<AActor*>& OutActors
	) const override;


};