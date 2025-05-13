// Pavel Penkov 2025 All Rights Reserved.

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
    FGameplayTag AbilityGameplayTagToCache;
};

/**
 * TODO clear cache on successfully activated ability? is it option?
 */
UCLASS()
class UHLGAS_API UAbilityInputCache final : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(Category = "AbilityInputCache")
    void SetUp(UUHLAbilitySystemComponent* ASC_In);

    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    bool AddTagToCache(FGameplayTag AbilityTag_In);
    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    bool AddTagsToCache(TArray<FGameplayTag> AbilityTags_In, bool bReverse = false);
    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    void CheckCache();
    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    void ClearCache();
    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    const TArray<FGameplayTag>& GetAbilityInputCache() const { return AbilityInputCache; };
    // TODO: remove tags from cache explicitly (probably bad practice, dont give option to do so?)
    // UFUNCTION(BlueprintCallable)
    // void RemoveTagFromCache(FGameplayTag AbilityTag_In);

private:
    UPROPERTY()
    TArray<FGameplayTag> AbilityInputCache = {};
    TWeakObjectPtr<UUHLAbilitySystemComponent> ASC;
};
