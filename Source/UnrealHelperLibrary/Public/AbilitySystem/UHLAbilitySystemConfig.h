// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UHLAbilitySystemConfig.generated.h"

class UGameplayAbility;
class UUHLAbilitySet;
class UUHLInputConfig;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FUHLAbilitySystemSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", meta=(InlineEditConditionToggle))
	bool bInitializeGameplayAttributes = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", meta=(ForceInlineRow, EditCondition="bInitializeGameplayAttributes"))
	TMap<FGameplayAttribute, float> InitialAttributes = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", meta=(InlineEditConditionToggle))
	bool bGiveAbilitiesOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystemSettings", meta=(EditCondition="bGiveAbilitiesOnStart"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", meta=(InlineEditConditionToggle))
	bool bGiveAttributesSetsOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystemSettings", meta=(EditCondition="bGiveAttributesSetsOnStart"))
	TArray<TSubclassOf<UAttributeSet>> AttributeSets = {};
	
	// TODO replace by "EUHLAbilityActivationPolicy::OnSpawn"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", meta=(InlineEditConditionToggle))
	bool bActivateAbilitiesOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystemSettings", meta=(EditCondition="bActivateAbilitiesOnStart"))
	TArray<FGameplayTagContainer> ActiveAbilitiesOnStart = {};
	
	// TODO initial GameplayEffects?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", meta=(InlineEditConditionToggle))
	bool bGiveInitialGameplayTags = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystemSettings", meta=(EditCondition="bGiveInitialGameplayTags"))
	FGameplayTagContainer InitialGameplayTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemSettings", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bGiveAbilitySetsOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystemSettings", AdvancedDisplay, meta=(EditCondition="bGiveAbilitySetsOnStart"))
	TArray<UUHLAbilitySet*> AbilitySets = {};
	
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category="AbilitySystemSettings", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bPreviewAllAbilities = true;
	UPROPERTY(VisibleDefaultsOnly, Transient, Category="AbilitySystemSettings", AdvancedDisplay, meta=(EditCondition="bPreviewAllAbilities", MultiLine=true))
	TMap<FString, FString> DebugPreviewAbilitiesFromAbilitySets = {};
	
	// binding inputs to tags check Readme.MD on how to setup it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig")
	bool bUseInputConfig = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="bUseInputConfig"))
	UUHLInputConfig* InputConfig = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="bUseInputConfig"))
	bool bUseAbilityInputCache = false;
	// if enabled - caching works only in predefined user windows - ANS_AbilityInputCache_CacheWindow
	// if disabled - works always
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="bUseInputConfig && bUseAbilityInputCache"))
	bool bUseInputCacheWindows = true;
};

/**
 * if you want to set default values like "AttributeSets" or something just nest from this
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALHELPERLIBRARY_API UUHLAbilitySystemConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UUHLAbilitySystemConfig();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilitySystemConfig", meta=(ShowOnlyInnerProperties))
	FUHLAbilitySystemSettings Settings;
};
