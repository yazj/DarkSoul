//  Copyright Yazj, All Rights Reserved

#pragma once

#include "DarkSoul.h"
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
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
	//UDarkSoulGameplayAbility();
	
};
