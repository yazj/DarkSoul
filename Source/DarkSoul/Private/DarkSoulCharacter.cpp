// Copyright Yazj, All Rights Reversed


#include "DarkSoulCharacter.h"
#include "Items/DarkSoulItem.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/DarkSoulGameplayAbility.h"

// Sets default values
ADarkSoulCharacter::ADarkSoulCharacter()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UDarkSoulAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UDarkSoulAttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;
	bAbilitiesInitialized = false;

}

void ADarkSoulCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Try setting the inventory source, this will fail for AI
	InventorySource = NewController;

	if (InventorySource)
	{
		InventoryUpdateHandle = InventorySource->GetSlottedItemChangedDelegate().AddUObject(this, &ADarkSoulCharacter::OnItemSlotChanged);
		InventoryLoadedHandle = InventorySource->GetInventoryLoadedDelegate().AddUObject(this, &ADarkSoulCharacter::RefreshSlottedGameplayAbilities);
	}

	// Initialize abilities
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}

}

void ADarkSoulCharacter::UnPossessed()
{
	// Unmap for inventory source
	if (InventorySource && InventoryUpdateHandle.IsValid())
	{
		InventorySource->GetSlottedItemChangedDelegate().Remove(InventoryUpdateHandle);
		InventoryUpdateHandle.Reset();

		InventorySource->GetInventoryLoadedDelegate().Remove(InventoryLoadedHandle);
		InventoryLoadedHandle.Reset();
	}

	InventorySource = nullptr;
}

void ADarkSoulCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void ADarkSoulCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADarkSoulCharacter, CharacterLevel);
}

UAbilitySystemComponent* ADarkSoulCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ADarkSoulCharacter::GetHealth() const
{
	return 0.0f;
}

float ADarkSoulCharacter::GetMaxHealth() const
{
	return 0.0f;
}

float ADarkSoulCharacter::GetMana() const
{
	return 0.0f;
}

float ADarkSoulCharacter::GetMaxMana() const
{
	return 0.0f;
}

float ADarkSoulCharacter::GetMoveSpeed() const
{
	return 0.0f;
}

int32 ADarkSoulCharacter::GetCharacterLevel() const
{
	return int32();
}

bool ADarkSoulCharacter::SetCharacterLevel(int32 NewLevel)
{
	return false;
}

bool ADarkSoulCharacter::ActivateAbilitiesWithItemSlot(FDarkSoulItemSlot ItemSlot, bool bAllowRemoteActivation)
{
	return false;
}

void ADarkSoulCharacter::GetActiveAbilitiesWithItemSlot(FDarkSoulItemSlot ItemSlot, TArray<UDarkSoulGameplayAbility*>& ActiveAbilities)
{
}

bool ADarkSoulCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	return false;
}

void ADarkSoulCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UDarkSoulGameplayAbility*>& ActiveAbilities)
{
}

bool ADarkSoulCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	return false;
}

void ADarkSoulCharacter::OnItemSlotChanged(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item)
{
	RefreshSlottedGameplayAbilities();
}

void ADarkSoulCharacter::RefreshSlottedGameplayAbilities()
{
}

void ADarkSoulCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		// Grant abilities, but only on the server
		for (TSubclassOf<UDarkSoulGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
		}

		// Now apply passive
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHanle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}

	}

	AddSlottedGameplayAbilities();

	bAbilitiesInitialized = true;

}

void ADarkSoulCharacter::RemoveStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && bAbilitiesInitialized)
	{
		// Remove any abilities added from a previous call
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && GameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		// Do in two passes so the removal happens after we have the full list
		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}

		// Remove all of the passive gameplay effects that were applied by this character
		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		AbilitySystemComponent->RemoveActiveEffects(Query);

		RemoveSlottedGameplayAbilities(true);

		bAbilitiesInitialized = false;
	}
}

void ADarkSoulCharacter::AddSlottedGameplayAbilities()
{
	TMap<FDarkSoulItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
	FillSlottedAbilitySpecs(SlottedAbilitySpecs);

	// Now add abilities if needed
	for (const TPair<FDarkSoulItemSlot, FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
	{
		FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);

		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
		}
	}

}

void ADarkSoulCharacter::FillSlottedAbilitySpecs(TMap<FDarkSoulItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs)
{
}

void ADarkSoulCharacter::RemoveSlottedGameplayAbilities(bool bRemoveAll)
{
	TMap<FDarkSoulItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;

	if (!bRemoveAll)
	{
		// Fill in map so we can compare
		FillSlottedAbilitySpecs(SlottedAbilitySpecs);
	}

	for (TPair<FDarkSoulItemSlot, FGameplayAbilitySpecHandle>& ExistingPair : SlottedAbilities)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(ExistingPair.Value);
		bool bShouldRemove = bRemoveAll || !FoundSpec;

		if (!bShouldRemove)
		{
			// Need to check desired ability specs, if we got here FoundSpec is valid
			FGameplayAbilitySpec* DesiredSpec = SlottedAbilitySpecs.Find(ExistingPair.Key);

			if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
			{
				bShouldRemove = true;
			}

		}

		if (bShouldRemove)
		{
			if (FoundSpec)
			{
				// Need to remove registered ability
				AbilitySystemComponent->ClearAbility(ExistingPair.Value);
			}
			// Make sure handle is cleared even if ability wasn't found
			ExistingPair.Value = FGameplayAbilitySpecHandle();
		}
	}
}

void ADarkSoulCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, ADarkSoulCharacter* InstigatorCharacter, AActor* DamageCauser)
{
}

void ADarkSoulCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}

void ADarkSoulCharacter::HandleManaChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}

void ADarkSoulCharacter::HandleMoveSpeedChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}

FGenericTeamId ADarkSoulCharacter::GetGenericTeamId() const
{
	return FGenericTeamId();
}


