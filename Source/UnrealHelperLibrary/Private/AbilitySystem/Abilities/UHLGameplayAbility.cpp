// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/UHLGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"

UUHLAbilitySystemComponent* UUHLGameplayAbility::GetUHLAbilitySystemComponentFromActorInfo() const
{
    if (!ensure(CurrentActorInfo))
    {
        return nullptr;
    }
    checkf(CastChecked<UUHLAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()), TEXT("UUHLGameplayAbility::GetUHLAbilitySystemComponentFromActorInfo can be used only on characters with UHLAbilitySystemComponent"));
    return StaticCast<UUHLAbilitySystemComponent*>(CurrentActorInfo->AbilitySystemComponent.Get());
}
