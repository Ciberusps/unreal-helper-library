// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UHLGameplayAbility.h"
#include "GA_AbilityInputCache.generated.h"


// UCLASS(Blueprintable)
// class UAbilityInputCachePayload : public UObject
// {
//     GENERATED_BODY()
//
// public:
//     UPROPERTY(EditAnywhere, BlueprintReadWrite)
//     FGameplayTag AbilityGameplayTagToCache;
// };

/**
 * TODO store more than one AbilityTag?
 * TODO move to UHLAbilitySystemComponent->AbilityInputCache UObject?? no need to have an ability for input caching
 * TODO add debug via console like "UHL.AbilityInputCache 1"
 */
UCLASS()
class UNREALHELPERLIBRARY_API UGA_AbilityInputCache : public UUHLGameplayAbility
{
	GENERATED_BODY()

public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UFUNCTION(BlueprintCallable)
    void AddTagToCache(FGameplayTag AbilityTag_In);
    UFUNCTION(BlueprintCallable)
    void CheckCache();
    UFUNCTION(BlueprintCallable)
    void ClearCache();
    // UFUNCTION(BlueprintCallable)
    // void RemoveTagFromCache(FGameplayTag AbilityTag_In);


private:
    UFUNCTION()
    void OnReceivedAddToInputCacheEvent(FGameplayEventData EventData);
    UFUNCTION()
    void OnReceivedCheckInputCacheEvent(FGameplayEventData EventData);

    UPROPERTY()
    TArray<FGameplayTag> AbilityInputCache = {};
};
