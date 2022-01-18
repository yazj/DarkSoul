// Copyright Yazj, All Rights Reversed

#include "DarkSoulBlueprintLibrary.h"

void UDarkSoulBlueprintLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{

}

void UDarkSoulBlueprintLibrary::StopLoadingScreen()
{
}

bool UDarkSoulBlueprintLibrary::IsInEditor()
{
	return false;
}

bool UDarkSoulBlueprintLibrary::Equal_DarkSoulItemSlot(const FDarkSoulItemSlot& A, const FDarkSoulItemSlot& B)
{
	return false;
}

bool UDarkSoulBlueprintLibrary::NotEqual_DarkSoulItemSlot(const FDarkSoulItemSlot& A, const FDarkSoulItemSlot& B)
{
	return false;
}

bool UDarkSoulBlueprintLibrary::IsValidItemSlot(const FDarkSoulItemSlot& ItemSlot)
{
	return false;
}

bool UDarkSoulBlueprintLibrary::DoesEffectContainerSpecHaveEffects(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec)
{
	return false;
}

bool UDarkSoulBlueprintLibrary::DoesEffectContainerSpecHaveTargets(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec)
{
	return false;
}

FDarkSoulGameplayEffectContainerSpec UDarkSoulBlueprintLibrary::AddTargetsToEffectContainerSpec(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	return FDarkSoulGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UDarkSoulBlueprintLibrary::ApplyExternalEffectContainerSpec(const FDarkSoulGameplayEffectContainerSpec& ContainerSpec)
{
	return TArray<FActiveGameplayEffectHandle>();
}

FString UDarkSoulBlueprintLibrary::GetProjectVersion()
{
	return FString();
}
