// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/UHLGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLGameplayAbility)

UUHLAbilitySystemComponent* UUHLGameplayAbility::GetUHLAbilitySystemComponentFromActorInfo() const
{
    if (!ensure(CurrentActorInfo))
    {
        return nullptr;
    }
    checkf(CastChecked<UUHLAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()), TEXT("UUHLGameplayAbility::GetUHLAbilitySystemComponentFromActorInfo can be used only on characters with UHLAbilitySystemComponent"));
    return StaticCast<UUHLAbilitySystemComponent*>(CurrentActorInfo->AbilitySystemComponent.Get());
}
