// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UHLGameplayAbility.generated.h"

class UUHLAbilitySystemComponent;

/**
 * EUHLAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EUHLAbilityActivationPolicy : uint8
{
    // Try to activate the ability when the input is triggered.
    OnInputTriggered,

    // Don't work, known issue, fix will come soon,
    // for now just use "WaitInputRelease" in GameplayAbility to check when input released and "EndAbility"
    //
    // Continually try to activate the ability while the input is active. To cancel ability use WaitInputRelease
    WhileInputActive,

    // Try to activate the ability when an avatar is assigned.
    OnSpawn
};

/**
 *
 */
UCLASS(Abstract, Category="UnrealHelperLibrary", Blueprintable, BlueprintType)
class UNREALHELPERLIBRARY_API UUHLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    EUHLAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

    // should cache input if ability can't be activated for now
    // Requirements:
    // - ASC should enable "bUseAbilityInputCache"
    // - Ability should have at least one AbilityTag
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache")
    bool bCacheInput = false;
    // if any of those tags on owner ASC - cache input
    // UPD not required we cache only abilities that can't be activated and have bCachedInput
    // so when CheckCache will be called we will try to activate them all, but latest - first
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache")
    FGameplayTagContainer AddingToCacheInputRequiredTags;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache")
    FGameplayTagContainer AddingToCacheInputBlockedTags;

    UFUNCTION(BlueprintCallable, Category = Ability)
    UUHLAbilitySystemComponent* GetUHLAbilitySystemComponentFromActorInfo() const;

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
    // Defines how this ability is meant to activate.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EUHLAbilityActivationPolicy ActivationPolicy = EUHLAbilityActivationPolicy::OnInputTriggered;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnPawnAvatarSet();

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();
	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();
	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();
};
