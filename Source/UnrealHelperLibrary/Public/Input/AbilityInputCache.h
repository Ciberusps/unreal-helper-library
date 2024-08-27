// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/NoExportTypes.h"
#include "AbilityInputCache.generated.h"


class UUHLAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogGA_AbilityInputCache, Log, All);

UCLASS(Blueprintable)
class UAbilityInputCachePayload : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag AbilityGameplayTagToCache;
};

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAbilityInputCache : public UObject
{
	GENERATED_BODY()

public:
    void SetUp(UUHLAbilitySystemComponent* ASC_In);

    UFUNCTION(BlueprintCallable)
    void AddTagToCache(FGameplayTag AbilityTag_In);
    UFUNCTION(BlueprintCallable)
    void CheckCache();
    UFUNCTION(BlueprintCallable)
    void ClearCache();
    UFUNCTION(BlueprintCallable)
    void DrawDebug();
    // UFUNCTION(BlueprintCallable)
    // void RemoveTagFromCache(FGameplayTag AbilityTag_In);

private:
    // UFUNCTION()
    void OnReceivedAddToInputCacheEvent(FGameplayTag GameplayTag_In, const FGameplayEventData* EventData);
    // UFUNCTION()
    void OnReceivedCheckInputCacheEvent(FGameplayTag GameplayTag_In, const FGameplayEventData* EventData);

    UPROPERTY()
    TArray<FGameplayTag> AbilityInputCache = {};
    TWeakObjectPtr<UUHLAbilitySystemComponent> ASC;
};
