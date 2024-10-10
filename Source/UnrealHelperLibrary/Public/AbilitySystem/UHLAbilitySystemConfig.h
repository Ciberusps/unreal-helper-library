// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UHLAbilitySystemConfig.generated.h"

struct FGameplayAttribute;
class UGameplayAbility;
class UUHLAbilitySet;
class UUHLInputConfig;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FUHLAbilitySystemSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
	bool bInitializeGameplayAttributes = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceInlineRow, EditCondition="bInitializeGameplayAttributes"))
	TMap<FGameplayAttribute, float> InitialAttributes = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
	bool bGiveAbilitiesOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveAbilitiesOnStart"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
	bool bGiveAttributesSetsOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveAttributesSetsOnStart"))
	TArray<TSubclassOf<UAttributeSet>> AttributeSets = {};
	
	// TODO replace by "EUHLAbilityActivationPolicy::OnSpawn"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
	bool bActivateAbilitiesOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bActivateAbilitiesOnStart"))
	TArray<FGameplayTagContainer> ActiveAbilitiesOnStart = {};
	
	// TODO initial GameplayEffects?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
	bool bGiveInitialGameplayTags = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveInitialGameplayTags"))
	FGameplayTagContainer InitialGameplayTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bGiveAbilitySetsOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, meta=(EditCondition="bGiveAbilitySetsOnStart"))
	TArray<UUHLAbilitySet*> AbilitySets = {};
	
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bPreviewAllAbilities = true;
	UPROPERTY(VisibleDefaultsOnly, Transient, AdvancedDisplay, meta=(EditCondition="bPreviewAllAbilities", MultiLine=true))
	TMap<FString, FString> DebugPreviewAbilitiesFromAbilitySets = {};
	
	// binding inputs to tags check Readme.MD on how to setup it
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="UHL InputConfig")
	bool bUseInputConfig = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL InputConfig", meta=(EditCondition="bUseInputConfig"))
	UUHLInputConfig* InputConfig = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL InputConfig", meta=(EditCondition="bUseInputConfig"))
	bool bUseAbilityInputCache = false;
	// if enabled - caching works only in predefined user windows - ANS_AbilityInputCache_CacheWindow
	// if disabled - works always
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL InputConfig", meta=(EditCondition="bUseInputConfig && bUseAbilityInputCache"))
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ShowOnlyInnerProperties))
	FUHLAbilitySystemSettings Settings;
};
