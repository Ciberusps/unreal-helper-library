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
	void OnAbilityActivate(UGameplayAbility *ActivatedAbility);
	UFUNCTION()
	void OnAbilityDeactivate(UGameplayAbility *ActivatedAbility);
	
	/**
	 * Wait until the specified gameplay attribute is changed on a target ability system component
	 * It will keep listening as long as OnlyTriggerOnce = false
	 * If used in an ability graph, this async action will wait even after activation ends. It's recommended to use WaitForAttributeChange instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_TryActivateAbilityAndWait* TryActivateAbilityAndWait(AActor* TargetActor, FGameplayTag WithTag,
		bool IncludeTriggeredAbilities=false, bool bAllowRemoteActivation=true, bool TriggerOnce=true);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityActivateDelegate, UGameplayAbility*, ActivatedAbility);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityActivateDelegate OnActivate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityDeactivateDelegate, UGameplayAbility*, ActivatedAbility);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityActivateDelegate OnEndAbility;

protected:
	virtual void Activate() override;
	virtual void EndAction() override;

	FGameplayTag WithTag;
	FGameplayTag WithoutTag;
	bool IncludeTriggeredAbilities = false;
	bool TriggerOnce;
	bool bAllowRemoteActivation = true;
	FGameplayTagRequirements TagRequirements;

	FGameplayTagQuery Query;

protected:
	FDelegateHandle OnAbilityActivateDelegateHandle;
};
