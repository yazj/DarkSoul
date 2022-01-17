// Copyright Yazj, All Rights Reversed

#pragma once

#include "DarkSoul.h"
#include "GameFramework/Character.h"
#include "UObject/ScriptInterface.h"
#include "DarkSoulInventoryInterface.h"
#include "AbilitySystemInterface.h"
#include "Abilities/DarkSoulAbilitySystemComponent.h"
#include "Abilities/DarkSoulAttributeSet.h"
#include "GenericTeamAgentInterface.h"
#include "DarkSoulCharacter.generated.h"

class UDarkSoulGameplayAbility;
class UGameplayEffect;

/** Base class for character, Designed to be blueprinted */
UCLASS()
class DARKSOUL_API ADarkSoulCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	ADarkSoulCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Return current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/** Returns current mana */
	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** Modifies the character level, this may change abilities. Returns true on success */
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

	/**
	*	Attempts to activate any ability in the specified item slot. Will return false if no activatable ability found or activation fails
	*   Returns true if it thinks it activated, but it may return false positive due to failure later in activation
	*	If bAllowRemoteActivation is true, it will remotely activate local/server abilities,
	*	otherwise it will only try to locally activate the ability
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithItemSlot(FDarkSoulItemSlot ItemSlot, bool bAllowRemoteActivation = true);
	
	/** Returns a list of active abilities bound to the item slot. This only returns if the ability is current running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithItemSlot(FDarkSoulItemSlot ItemSlot,
		TArray<UDarkSoulGameplayAbility*>& ActiveAbilities);

	/**
	*	Attempts to active all abilities that match the specified tags
	*	Returns true if it thinks it activated, but it may return false positive due to failure later in activation.
	*	If bAllowRemoteActivation is true, it will remotely activate local/server abilities,
	*	otherwise it will only try to locally activate the ability
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	/** Returns a list of active abilities matching the specified tags. This only returns if the ability is current running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags,
		TArray<UDarkSoulGameplayAbility*>& ActiveAbilities);

	/** Returns total time and remaining time for cooldown tags. Return false if no active cooldown found */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags,
		float& TimeRemaining,
		float& CooldownDuration);

protected:	
	/** The level of this character, should not be modified directly once it has already spawned */
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	/** Abilities to grant to this character on creation.
	*	This will be activated by tag or event and are not bound to specific inputs
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UDarkSoulGameplayAbility>> GameplayAbilities;

	/** Map of item slot to gameplay ability class, these are bound any abilities added by the inventory  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TMap<FDarkSoulItemSlot, TSubclassOf<UDarkSoulGameplayAbility>> DefaultSlottedAbilities;

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** The component used to handle ability system interactions */
	UPROPERTY()
	UDarkSoulAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	UDarkSoulAttributeSet* AttributeSet;

	/** Cached pointer to the inventory source for this character, can be null */
	UPROPERTY()
	TScriptInterface<IDarkSoulInventoryInterface> InventorySource;

	/** If true we have initialized our abilities */
	UPROPERTY()
	int32 bAbilitiesInitialized;

	/** Map of slot to ability granted by the slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FDarkSoulItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;

	/** Delegate handles */
	FDelegateHandle InventoryUpdateHandle;
	FDelegateHandle InventoryLoadedHandle;

	/**
	*	Called when character takes damage, which may have killed them
	* 
	*	@param DamageAmount Amount of damage that was done, not clamped based on current health
	*	@param HitInfo The hit info that generated this damage
	*	@param DamageTags the gameplay tags of the event that did the damage
	*	@param InstigatorCharacter the character that initiated this damage
	*	@param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	*/
	UFUNCTION(BlueprintimplementableEvent)
	void OnDamaged(
		float DamageAmount, 
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags, 
		ADarkSoulCharacter* InstigatorCharacter,
		AActor* DamageCauser);

	/**
	*	Called when health is changed, either from healing or from being damaged
	*	For damage this is called in addition to OnDamaged/OnKilled
	* 
	*	@param DeltaValue Change in health value, positive for heal, negative for cost.If 0 the delta is unknown
	*	@param EventTags the gameplay tags of the event that changed health
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	*	Called when mana is changed, either from healing or from being damaged
	*
	*	@param DeltaValue Change in health value, positive for heal, negative for cost.If 0 the delta is unknown
	*	@param EventTags the gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	*	Called when movement speed is changed
	* 
	*	@param DeltaValue Change in move speed
	*	@param EventTags The gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/** Called when slotted items change, bound to delegate on interface */
	void OnItemSlotChanged(FDarkSoulItemSlot ItemSlot, UDarkSoulItem* Item);
	void RefreshSlottedGameplayAbilities();

	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

	/** Attempts to remove any startup gameplay abilities */
	void RemoveStartupGameplayAbilities();

	/** Adds slotted item abilities if needed */
	void AddSlottedGameplayAbilities();

	/** Fills in with ability specs, based on defaults and inventory */
	void FillSlottedAbilitySpecs(TMap<FDarkSoulItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs);

	/** Remove slotted gameplay abilities, if force is false it only removes invalid ones */
	void RemoveSlottedGameplayAbilities(bool bRemoveAll);

	// Called from DarkSoulAttributeSet, these call BP events above
	virtual void HandleDamage(
		float DamageAmount,
		const FHitResult& HitInfo,
		const struct FGameplayTagContainer& DamageTags,
		ADarkSoulCharacter* InstigatorCharacter,
		AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/** Required to support AIPerceptionSystem */
	virtual FGenericTeamId GetGenericTeamId() const override;

	// Friended to allow access to handle functions above
	friend UDarkSoulAttributeSet;

};
