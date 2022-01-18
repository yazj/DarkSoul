// Copyright Yazj, All Rights Reversed

#pragma once

#include "DarkSoul.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DarkSoulDamageExecution.generated.h"

/**
 * A damage execution, which allows doing damage by combining a raw Damage number with AttackPower and DefensePower
 * Most games will want to implement multiple game-specific executions
 */
UCLASS()
class DARKSOUL_API UDarkSoulDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	// Constructor and overrides
	UDarkSoulDamageExecution();
	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};
