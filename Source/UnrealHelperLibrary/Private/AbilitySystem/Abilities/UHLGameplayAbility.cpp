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

void UUHLGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    K2_OnAbilityAdded();

    // TODO activated ability OnSpawn
    // TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UUHLGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    K2_OnAbilityRemoved();
    Super::OnRemoveAbility(ActorInfo, Spec);
}

void UUHLGameplayAbility::OnPawnAvatarSet()
{
    K2_OnPawnAvatarSet();
}
