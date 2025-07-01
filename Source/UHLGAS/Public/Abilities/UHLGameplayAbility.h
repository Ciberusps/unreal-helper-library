// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UHLGameplayAbility.generated.h"

class UUHLAbilitySystemComponent;

/**
 * EUHLAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EUHLAbilityActivationPolicy : uint8
{
    // Try to activate the ability when the input is triggered.
    OnInputTriggered,
	
    // Continually try to activate the ability while the input is active.
    // Subscribe on "WaitInputRelease" and "EndAbility" in blueprint,
    // it's not possible to EndAbility from C++
    WhileInputActive,

    // Try to activate the ability when an avatar is assigned.
    OnSpawn
};

// USTRUCT(BlueprintType)
// struct FUHLWhileInputActiveSettings
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	bool bCancelAbilityAutomatically = true;
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	bool bReplicateEndAbility = true;
// 	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	// bool bMarkAsCanceledOnEnd = false;
// };

/**
 *
 */
UCLASS(Abstract, Category="UnrealHelperLibrary", Blueprintable, BlueprintType)
class UHLGAS_API UUHLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UUHLGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
    UFUNCTION(BlueprintCallable, Category = "UHL GameplayAbility")
    EUHLAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	// UFUNCTION(BlueprintCallable)
	// FUHLWhileInputActiveSettings GetWhileInputActiveSettings() const { return WhileInputActiveSettings; }
	
    // should cache input if ability can't be activated for now
    // Requirements:
    // - ASC should enable "bUseAbilityInputCache"
    // - Ability should have at least one AbilityTag
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache")
    bool bCacheInput = false;

	// ignore fact that ability already activated while checking cache
	// it means
	// - ability should be added to InputCache even if its already activated
	// - "bRetriggerInstancedAbility" setting should be disabled to work because during
	//    AbilityInputCache->CheckCache() we cancel previous ability and start it again
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache")
	bool bIgnoreActivatedState = false;

    // tags required on owner ASC to be cache ability
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache", DisplayName="[Add to Cache] RequiredTags")
    FGameplayTagContainer AddToCacheRequiredTags;
	// tags blocked on owner ASC - if present ability won't be cached
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInputCache", DisplayName="[Add to Cache] BlockedTags")
    FGameplayTagContainer AddToCacheBlockedTags;

	/**
	* When true, external CancelAbility calls are intercepted and must be completed manually via ReleaseCancellation().
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UHL GameplayAbility")
	bool bCancelManually = false;

	UFUNCTION(BlueprintCallable, Category="UHL GameplayAbility")
	bool GetIsCancelRequested() const { return bCancelRequested; };

    UFUNCTION(BlueprintCallable, Category="UHL GameplayAbility")
    UUHLAbilitySystemComponent* GetUHLAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category="Ability", DisplayName = "CommitAbilityDuration")
	bool K2_CommitAbilityDuration(bool BroadcastCommitEvent);
	
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	// Commit Ability
	virtual void CommitExecute(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual bool CommitAbilityDuration(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr);

	void ApplyDuration(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo);
	bool CheckAbilityDuration(
		FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags) const;

	UFUNCTION(BlueprintNativeEvent, Category = "UHL GameplayAbility")
	void OnDurationEnd(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo);

	const FGameplayTagContainer* GetAbilityDurationTags() const;
	virtual UGameplayEffect* GetAbilityDurationGameplayEffect() const;

	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
	// ~Commit Ability
	
	/**
	* Completes a pending cancel request immediately. Call this to actually end the ability when bCancelManually is true.
	*/
	UFUNCTION(BlueprintCallable, Category = "UHL GameplayAbility")
	void ReleaseCancellation();
	
protected:
    // Defines how this ability is meant to activate.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UHL GameplayAbility")
    EUHLAbilityActivationPolicy ActivationPolicy = EUHLAbilityActivationPolicy::OnInputTriggered;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="ActivationPolicy == EUHLAbilityActivationPolicy::WhileInputActive", EditConditionHides))
	// FUHLWhileInputActiveSettings WhileInputActiveSettings;

	
	/** Tracks whether a cancel request has been received */
	bool bCancelRequested = false;

#if WITH_EDITORONLY_DATA
	/** Timer handle for editor warning if ReleaseCancellation isn't called in time */
	FTimerHandle EditorWarningHandle;
#endif

	/**
	 * Override external cancel: either intercept or pass through.
	 */
	virtual void CancelAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility = true) override;

	/**
	 * Called when a manual cancel request is intercepted.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Cancellation", meta = (DisplayName = "OnCancelRequested"))
	void ReceiveCancelRequested();

	/**
	 * Called when cancellation is completed (after manual release).
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Cancellation", meta = (DisplayName = "OnCancelCompleted"))
	void ReceiveCancelCompleted();
	
#if WITH_EDITORONLY_DATA
	/**
	 * Editor-only: checks if cancellation was never released and draws a warning.
	 */
	void CheckCancelReminder();
#endif
	

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnPawnAvatarSet();

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category="UHL GameplayAbility", DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();
	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category="UHL GameplayAbility", DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();
	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category="UHL GameplayAbility", DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

private:
	UPROPERTY(EditDefaultsOnly, Category="Ability Duration")
	TSubclassOf<class UGameplayEffect> AbilityDurationGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Ability Duration")
	bool bEndAbilityOnDurationExpired = true;
};
