// Copyright Yazj. All Rights Reserved.

#include "Abilities/DarkSoulTargetType.h"
#include "Abilities/DarkSoulGameplayAbility.h"
#include "DarkSoulCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void UDarkSoulTargetType::GetTargets_Implementation(ADarkSoulCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void UDarkSoulTargteType_UseOwner::GetTargets_Implementation(ADarkSoulCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void UDarkSoulTargteType_UseEventData::GetTargets_Implementation(ADarkSoulCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}

