// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UHLAbilitySet.h"
#include "Input/AbilityInputCache.h"
#include "UHLAbilitySystemComponent.generated.h"

class UUHLAbilitySet;
class UUHLInputConfig;
class UUHLGameplayAbility;

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary", Blueprintable, meta=(BlueprintSpawnableComponent))
class UNREALHELPERLIBRARY_API UUHLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
    bool bInitializeGameplayAttributes = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
    bool bGiveAbilitiesOnStart = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
    bool bActivateAbilitiesOnStart = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
    bool bGiveAttributesSetsOnStart = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
    bool bGiveInitialGameplayTags = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceInlineRow, EditCondition="bInitializeGameplayAttributes"))
    TMap<FGameplayAttribute, float> InitialAttributes = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveAbilitiesOnStart"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveAttributesSetsOnStart"))
    TArray<TSubclassOf<UAttributeSet>> AttributeSets;
	// TODO replace by "EUHLAbilityActivationPolicy::OnSpawn"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bActivateAbilitiesOnStart"))
	TArray<FGameplayTagContainer> ActiveAbilitiesOnStart;
    // TODO initial GameplayEffects?
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveInitialGameplayTags"))
    FGameplayTagContainer InitialGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bGiveAbilitySetsOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, meta=(EditCondition="bGiveAbilitySetsOnStart"))
	TArray<UUHLAbilitySet*> AbilitySets;
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bPreviewAllAbilities = true;
	UPROPERTY(VisibleDefaultsOnly, Transient, AdvancedDisplay, meta=(EditCondition="bPreviewAllAbilities"))
	TMap<FString, FString> DebugPreviewAbilitiesFromAbilitySets;

    // binding inputs to tags check Readme.MD on how to setup it
    UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="UHL InputConfig")
    bool bUseInputConfig = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL InputConfig", meta=(EditCondition="bUseInputConfig"))
    UUHLInputConfig* InputConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL InputConfig", meta=(EditCondition="bUseInputConfig"))
    bool bUseAbilityInputCache = false;
    // if enabled - caching works only in predefined user windows - ANS_AbilityInputCache_CacheWindow
    // if disabled - works always
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL InputConfig", meta=(EditCondition="bUseInputConfig && bUseAbilityInputCache"))
    bool bUseInputCacheWindows = true;

    UFUNCTION(BlueprintCallable)
    virtual void InitAbilitySystem(AActor* NewOwner, AActor* InAvatarActor, bool bActivateInitialAbilities = true);
    UFUNCTION(BlueprintCallable)
	virtual void InitAttributes();
    UFUNCTION(BlueprintCallable)
	virtual void SetAttributes(TMap<FGameplayAttribute, float> Attributes_In);
    UFUNCTION(BlueprintCallable)
	virtual void ActivateInitialAbilities();

	UFUNCTION(BlueprintCallable)
	virtual void GiveAbilitySet(const UUHLAbilitySet* AbilitySet);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveAbilitySetByTag(const FGameplayTag& GameplayTag);

/** Input Config **/
    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void AbilityInputTagPressed(const FGameplayTag InputTag);
	virtual void AbilityInputTagReleased(const FGameplayTag InputTag);
/** Input Config **/

    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    UAbilityInputCache* GetAbilityInputCache() const { return AbilityInputCache; };
    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    bool CanAddAbilityToCache(UUHLGameplayAbility* GameplayAbility_In) const;

	UFUNCTION(BlueprintCallable)
	virtual bool TryActivateAbilityWithTag(FGameplayTag GameplayTag, bool bAllowRemoteActivation = true);
	UFUNCTION(BlueprintCallable)
	virtual bool TryCancelAbilityWithTag(FGameplayTag GameplayTag);
	UFUNCTION(BlueprintCallable)
	virtual TArray<bool> TryCancelAbilitiesWithTags(TArray<FGameplayTag> GameplayTags);
    UFUNCTION(BlueprintCallable)
    virtual void RemoveLooseGameplayTagCompletly(const FGameplayTag& GameplayTag);
    UFUNCTION(BlueprintCallable)
    virtual bool IsAbilityActive(FGameplayTag GameplayTag) const;

	UFUNCTION(BlueprintCallable)
	virtual int32 FireGameplayEvent(FGameplayTag EventTag, const FGameplayEventData& Payload);

protected:
    virtual void BeginPlay() override;
    virtual void OnUnregister() override;

#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	void UpdatePreviewAbilitiesMap();
#endif

private:
    UPROPERTY()
    TObjectPtr<UAbilityInputCache> AbilityInputCache;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	// AbilitySets given to ASC 
	TArray<FUHLAbilitySet_GrantedHandles> AbilitySetGrantedHandles;
};
