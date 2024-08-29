// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilityInputCache.generated.h"


class UUHLAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(Log_UHL_AbilityInputCache, Log, All);

UCLASS(Blueprintable)
class UAbilityInputCachePayload : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag AbilityGameplayTagToCache;
};

/**
 * TODO clear cache on successfully activated ability? is it option?
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAbilityInputCache : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void SetUp(UUHLAbilitySystemComponent* ASC_In);

    UFUNCTION(BlueprintCallable)
    bool AddTagToCache(FGameplayTag AbilityTag_In);
    UFUNCTION(BlueprintCallable)
    bool AddTagsToCache(TArray<FGameplayTag> AbilityTags_In, bool bReverse = false);
    UFUNCTION(BlueprintCallable)
    void CheckCache();
    UFUNCTION(BlueprintCallable)
    void ClearCache();
    UFUNCTION(BlueprintCallable)
    TArray<FGameplayTag> GetAbilityInputCache() const { return AbilityInputCache; };
    // TODO: remove tags from cache explicitly (probably bad practice, dont give option to do so?)
    // UFUNCTION(BlueprintCallable)
    // void RemoveTagFromCache(FGameplayTag AbilityTag_In);

private:
    UPROPERTY()
    TArray<FGameplayTag> AbilityInputCache = {};
    TWeakObjectPtr<UUHLAbilitySystemComponent> ASC;
};
