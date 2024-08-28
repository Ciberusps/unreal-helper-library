// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_AbilityInputCache.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Core/UHLGameplayTags.h"
#include "Input/AbilityInputCache.h"
#include "Utils/UnrealHelperLibraryBPL.h"


void UGA_AbilityInputCache::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_WaitGameplayEvent* AbilityTask_WaitAddToInputCache = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UHLGameplayTags::TAG_UHL_AbilityInputCache_Add, nullptr, false, false);
    AbilityTask_WaitAddToInputCache->EventReceived.AddDynamic(this, &UGA_AbilityInputCache::OnReceivedAddToInputCacheEvent);
    AbilityTask_WaitAddToInputCache->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent* AbilityTask_WaitCheckInputCache = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UHLGameplayTags::TAG_UHL_AbilityInputCache_Add, nullptr, false, false);
    AbilityTask_WaitCheckInputCache->EventReceived.AddDynamic(this, &UGA_AbilityInputCache::OnReceivedCheckInputCacheEvent);
    AbilityTask_WaitCheckInputCache->ReadyForActivation();
}

void UGA_AbilityInputCache::OnReceivedAddToInputCacheEvent(FGameplayEventData EventData)
{
    const UAbilityInputCachePayload* Payload = Cast<UAbilityInputCachePayload>(EventData.OptionalObject);
    if (!Payload) return;

    AddTagToCache(Payload->AbilityGameplayTagToCache);
}

void UGA_AbilityInputCache::OnReceivedCheckInputCacheEvent(FGameplayEventData EventData)
{
    CheckCache();
}

void UGA_AbilityInputCache::AddTagToCache(FGameplayTag AbilityTag_In)
{
    // not required to be unique
    AbilityInputCache.Add(AbilityTag_In);
    UE_LOG(LogGA_AbilityInputCache, Log, TEXT("Add ability to InputCache - %s"), *AbilityTag_In.ToString());
}

// should cancel all abilities that also cached but not fired
void UGA_AbilityInputCache::CheckCache()
{
    UE_LOG(LogGA_AbilityInputCache, Log, TEXT("Check AbilityInputCache"));

    if (AbilityInputCache.IsEmpty())
    {
        UE_LOG(LogGA_AbilityInputCache, Log, TEXT("AbilityInputCache is empty"));
        return;
    };

    UAbilitySystemComponent* ASC = GetActorInfo().AbilitySystemComponent.Get();
    if (ASC)
    {
        FGameplayTag AbilityTagToActivate = AbilityInputCache.Last();
        if (AbilityTagToActivate.IsValid())
        {
			FGameplayEventData GameplayEventData = FGameplayEventData();
			// UAbilityInputCachePayload* AbilityInputCachePayload = NewObject<UAbilityInputCachePayload>();
			// AbilityInputCachePayload->AbilityGameplayTagToCache = AbilityTags.First();
			// GameplayEventData.OptionalObject = AbilityInputCachePayload;
			ASC->HandleGameplayEvent(AbilityTagToActivate, &GameplayEventData);
            UE_LOG(LogGA_AbilityInputCache, Log, TEXT("InputCache checked found ability to activate - %s!"), *AbilityTagToActivate.ToString());
       }
    }

    ClearCache();
    UE_LOG(LogGA_AbilityInputCache, Log, TEXT("AbilityInputCache clean"));
}

void UGA_AbilityInputCache::ClearCache()
{
    AbilityInputCache.Empty();
}

// void UGA_AbilityInputCache::RemoveTagFromCache(FGameplayTag AbilityTag_In)
// {
//     AbilityInputCache.Remove(AbilityTag_In);
// }
