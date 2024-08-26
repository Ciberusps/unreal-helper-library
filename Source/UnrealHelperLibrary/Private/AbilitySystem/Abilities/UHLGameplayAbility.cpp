// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/UHLGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/GA_AbilityInputCache.h"
#include "Core/UHLGameplayTags.h"

void UUHLGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    // // точно не до Super должно быть?
    // if (ASC && bCacheInput && ASC->HasAnyMatchingGameplayTags(CacheInputActivationTags))
    // {
    //     FGameplayEventData GameplayEventData = FGameplayEventData();
    //     UAbilityInputCachePayload* AbilitiesInputCachePayload = NewObject<UAbilityInputCachePayload>();
    //     AbilitiesInputCachePayload->AbilityGameplayTagToCache = AbilityTags.First();
    //     // Paylaod.Instigator = CachedBaseCharacter.Get();
    //     GameplayEventData.OptionalObject = AbilitiesInputCachePayload;
    //     ASC->HandleGameplayEvent(UHLGameplayTags::TAG_UHL_AbilityInputCache_Add, &GameplayEventData);
    // }
}
