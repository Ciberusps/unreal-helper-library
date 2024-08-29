// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UHLGameplayAbility.generated.h"

/**
 * EGCAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EUHLAbilityActivationPolicy : uint8
{
    // Try to activate the ability when the input is triggered.
    OnInputTriggered,

    // Continually try to activate the ability while the input is active. To cancel ability use WaitInputRelease
    WhileInputActive,

    // Try to activate the ability when an avatar is assigned.
    // OnSpawn
};

/**
 *
 */
UCLASS(Category="UnrealHelperLibrary", Blueprintable)
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

protected:
    // Defines how this ability is meant to activate.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EUHLAbilityActivationPolicy ActivationPolicy = EUHLAbilityActivationPolicy::OnInputTriggered;
};
