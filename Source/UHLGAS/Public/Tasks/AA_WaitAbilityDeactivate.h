// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "AA_WaitAbilityDeactivate.generated.h"

/**
 * 
 */
UCLASS()
class UHLGAS_API UAA_WaitAbilityDeactivate : public UAbilityAsync
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void OnAbilityDeactivate(UGameplayAbility *DeactivatedAbility);
	
	/**
	 * Wait until the specified gameplay attribute is changed on a target ability system component
	 * It will keep listening as long as OnlyTriggerOnce = false
	 * If used in an ability graph, this async action will wait even after activation ends. It's recommended to use WaitForAttributeChange instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAbilityDeactivate* WaitForAbilityDeactivate(AActor* TargetActor, FGameplayTag WithTag,
		FGameplayTag WithoutTag, bool TriggerOnce=true);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAbilityDeactivate* WaitForAbilityDeactivateWithTagRequirements(AActor* TargetActor,
		FGameplayTagRequirements TagRequirements, bool TriggerOnce = true);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAbilityDeactivate* WaitForAbilityDeactivate_Query(AActor* TargetActor,
		FGameplayTagQuery Query, bool TriggerOnce = true);
	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityDeactivateDelegate, UGameplayAbility*, ActivatedAbility);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityDeactivateDelegate OnDeactivate;

protected:
	virtual void Activate() override;
	virtual void EndAction() override;

	FGameplayTag WithTag;
	FGameplayTag WithoutTag;
	bool TriggerOnce;
	FGameplayTagRequirements TagRequirements;

	FGameplayTagQuery Query;

protected:
	FDelegateHandle OnAbilityDeactivateDelegateHandle;
};
