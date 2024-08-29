// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Input/AbilityInputCache.h"
#include "UHLAbilitySystemComponent.generated.h"

class UUHLGameplayAbility;
/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary", Blueprintable, meta=(BlueprintSpawnableComponent))
class UNREALHELPERLIBRARY_API UUHLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bInitializeGameplayAttributes = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGiveAbilitiesOnStart = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bActivateAbilitiesOnStart = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGiveInitialGameplayTags = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceInlineRow, EditCondition="bInitializeGameplayAttributes"))
    TMap<FGameplayAttribute, float> InitialGameplayAttributes = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveAbilitiesOnStart"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bActivateAbilitiesOnStart"))
	TArray<FGameplayTagContainer> InitialActiveAbilities;
    // TODO initial GameplayEffects?
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bGiveInitialGameplayTags"))
    FGameplayTagContainer InitialGameplayTags;

    // binding inputs to tags check Readme.MD on how to setup it
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseInputConfig = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bUseInputConfig"))
    bool bUseAbilityInputCache = true;
    // if enabled - caching works only in predefined user windows - ANS_AbilityInputCache_CacheWindow
    // if disabled - works always
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bUseAbilityInputCache"))
    bool bUseInputCacheWindows = true;

	virtual void BeginPlay() override;

	virtual void InitAbilitySystem(TObjectPtr<AController> NewController, TObjectPtr<AActor> InAvatarActor);
	virtual void InitAbilitySystem(AActor* NewOwner, AActor* InAvatarActor);
	virtual void InitAttributes();
	virtual void SetAttributes(TMap<FGameplayAttribute, float> Attributes_In);
	virtual void ActivateInitialAbilities();
	virtual void OnUnregister() override;

/** Input Config **/
    // TODO DA_InputConfig, UHLGameplayAbility
    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void AbilityInputTagPressed(const FGameplayTag& InputTag);
	virtual void AbilityInputTagReleased(const FGameplayTag& InputTag);
/** Input Config **/

    UFUNCTION(BlueprintCallable)
    UAbilityInputCache* GetAbilityInputCache() const { return AbilityInputCache; };

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

private:
    UPROPERTY()
    TObjectPtr<UAbilityInputCache> AbilityInputCache;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

    UFUNCTION(BlueprintCallable, Category = "AbilityInputCache")
    bool CanAddAbilityToCache(UUHLGameplayAbility* GameplayAbility_In) const;
};
