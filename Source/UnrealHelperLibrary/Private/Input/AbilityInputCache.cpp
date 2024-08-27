// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AbilityInputCache.h"

#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Core/UHLGameplayTags.h"


DEFINE_LOG_CATEGORY(LogGA_AbilityInputCache);

void UAbilityInputCache::SetUp(UUHLAbilitySystemComponent* ASC_In)
{
    if (!ASC.IsValid()) return;

    ASC = ASC_In;

    // ASC.Get()->AddGameplayEventTagContainerDelegate(
    //     FGameplayTagContainer(UHLGameplayTags::TAG_UHL_AbilityInputCache_Add),
    //     FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityInputCache::OnReceivedAddToInputCacheEvent));
    //
    // ASC.Get()->AddGameplayEventTagContainerDelegate(
    //     FGameplayTagContainer(UHLGameplayTags::TAG_UHL_AbilityInputCache_Add),
    //     FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityInputCache::OnReceivedAddToInputCacheEvent));

    // NOT required we already have AddTagToCache

    // UAbilityAsync_WaitGameplayEvent::
    // UAbilityAsync_WaitGameplayEvent* AbilityTask_WaitAddToInputCache = UAbilityAsync_WaitGameplayEvent::WaitGameplayEventToActor(ASC->GetOwner(), UHLGameplayTags::TAG_UHL_AbilityInputCache_Add, false, false);
    // AbilityTask_WaitAddToInputCache->EventReceived.AddDynamic(this, &UAbilityInputCache::OnReceivedAddToInputCacheEvent);
    // AbilityTask_WaitAddToInputCache->ReadyForActivation();

    // UAbilityTask_WaitGameplayEvent* AbilityTask_WaitCheckInputCache = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UHLGameplayTags::TAG_UHL_AbilityInputCache_Add, nullptr, false, false);
    // AbilityTask_WaitCheckInputCache->EventReceived.AddDynamic(this, &UAbilityInputCache::OnReceivedCheckInputCacheEvent);
    // AbilityTask_WaitCheckInputCache->ReadyForActivation();
}

void UAbilityInputCache::OnReceivedAddToInputCacheEvent(FGameplayTag GameplayTag_In, const FGameplayEventData* EventData)
{
    const UAbilityInputCachePayload* Payload = Cast<UAbilityInputCachePayload>(EventData->OptionalObject);
    if (!Payload) return;

    AddTagToCache(Payload->AbilityGameplayTagToCache);
}

void UAbilityInputCache::OnReceivedCheckInputCacheEvent(FGameplayTag GameplayTag_In, const FGameplayEventData* EventData)
{
    CheckCache();
}

void UAbilityInputCache::AddTagToCache(FGameplayTag AbilityTag_In)
{
    // not required to be unique
    AbilityInputCache.Add(AbilityTag_In);
    UE_LOG(LogGA_AbilityInputCache, Log, TEXT("Add ability to InputCache - %s"), *AbilityTag_In.ToString());
}

// should cancel all abilities that also cached but not fired
void UAbilityInputCache::CheckCache()
{
    UE_LOG(LogGA_AbilityInputCache, Log, TEXT("Check AbilityInputCache"));

    if (AbilityInputCache.IsEmpty())
    {
        UE_LOG(LogGA_AbilityInputCache, Log, TEXT("AbilityInputCache is empty"));
        return;
    };

    if (ASC.IsValid())
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

void UAbilityInputCache::ClearCache()
{
    AbilityInputCache.Empty();
}

void UAbilityInputCache::DrawDebug()
{

}

// void UAbilityInputCache::RemoveTagFromCache(FGameplayTag AbilityTag_In)
// {
//     AbilityInputCache.Remove(AbilityTag_In);
// }
