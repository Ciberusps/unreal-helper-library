// Pavel Penkov 2025 All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "AA_TryActivateAbilityAndWait.generated.h"

/**
 * 
 */
UCLASS()
class UHLGAS_API UAA_TryActivateAbilityAndWait : public UAbilityAsync
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnAbilityActivate(UGameplayAbility* ActivatedAbility);
	UFUNCTION()
	void OnAbilityDeactivate(UGameplayAbility *ActivatedAbility);

	/*
	UFUNCTION()
	void OnAbilityActivationFailed(const UGameplayAbility *ActivatedAbility, const FGameplayTagContainer& GameplayTagContainer);
	*/

	
	/**
	 * Wait until the specified gameplay attribute is changed on a target ability system component
	 * It will keep listening as long as OnlyTriggerOnce = false
	 * If used in an ability graph, this async action will wait even after activation ends. It's recommended to use WaitForAttributeChange instead.
	 *
	 * also to wait for activate/endability "WithTag" should be GameplayAbility "AbilityTags"(not strict tag check)
	 * TODO: "PayloadInfo" not great solution probably better to check AN_FireGameplayEvent with its OptionalObject1/2
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_TryActivateAbilityAndWait* TryActivateAbilityAndWait(AActor* TargetActor, FGameplayTagQuery TagQuery, bool bAllowRemoteActivation=true);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityActivateDelegate, UGameplayAbility*, ActivatedAbility);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityActivateDelegate OnActivate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityDeactivateDelegate, UGameplayAbility*, ActivatedAbility);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityActivateDelegate OnEndAbility;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitAbilityActivationFailedDelegate);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityActivationFailedDelegate OnActivationFailed;

protected:
	virtual void Activate() override;
	virtual void EndAction() override;

protected:
	FGameplayTagQuery TagQuery;
	bool bAllowRemoteActivation = true;
	FGameplayTagRequirements TagRequirements;

	//@TODO: Оба делегата были в одном хендле, мб вернуть?
	FDelegateHandle OnAbilityActivateDelegateHandle;
	FDelegateHandle OnAbilityDeactivateDelegateHandle;

private:
	FGameplayAbilitySpecHandle TrackedAbilitySpecHandle;
};
