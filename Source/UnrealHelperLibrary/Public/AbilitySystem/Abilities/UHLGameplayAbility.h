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
UCLASS(Blueprintable)
class UNREALHELPERLIBRARY_API UUHLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    EUHLAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
    // Defines how this ability is meant to activate.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EUHLAbilityActivationPolicy ActivationPolicy = EUHLAbilityActivationPolicy::OnInputTriggered;
};
