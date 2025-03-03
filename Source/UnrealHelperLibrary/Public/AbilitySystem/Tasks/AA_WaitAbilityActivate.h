// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "AA_WaitAbilityActivate.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAA_WaitAbilityActivate : public UAbilityAsync
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void OnAbilityActivate(UGameplayAbility *ActivatedAbility);
	
	/**
	 * Wait until the specified gameplay attribute is changed on a target ability system component
	 * It will keep listening as long as OnlyTriggerOnce = false
	 * If used in an ability graph, this async action will wait even after activation ends. It's recommended to use WaitForAttributeChange instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAbilityActivate* WaitForAbilityActivate(AActor* TargetActor, FGameplayTag WithTag,
		FGameplayTag WithoutTag, bool IncludeTriggeredAbilities=false, bool TriggerOnce=true);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAbilityActivate* WaitForAbilityActivateWithTagRequirements(AActor* TargetActor,
		FGameplayTagRequirements TagRequirements, bool IncludeTriggeredAbilities = false,
		bool TriggerOnce = true);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAbilityActivate* WaitForAbilityActivate_Query(AActor* TargetActor,
		FGameplayTagQuery Query, bool IncludeTriggeredAbilities = false, bool TriggerOnce = true);
	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityActivateDelegate, UGameplayAbility*, ActivatedAbility);
	UPROPERTY(BlueprintAssignable)
	FWaitAbilityActivateDelegate OnActivate;

protected:
	virtual void Activate() override;
	virtual void EndAction() override;

	FGameplayTag WithTag;
	FGameplayTag WithoutTag;
	bool IncludeTriggeredAbilities = false;
	bool TriggerOnce;
	FGameplayTagRequirements TagRequirements;

	FGameplayTagQuery Query;

protected:

	FDelegateHandle OnAbilityActivateDelegateHandle;
};
