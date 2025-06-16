// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeRatioThreshold.h"
#include "AA_WaitAttributeChangeRatioThreshold.generated.h"

struct FGameplayEffectModCallbackData;

/**
 * 
 */
UCLASS()
class UHLGAS_API UAA_WaitAttributeChangeRatioThreshold : public UAbilityAsync
{
	GENERATED_BODY()

	UAA_WaitAttributeChangeRatioThreshold(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable)
	FWaitAttributeChangeRatioThresholdDelegate OnChange;


	void OnNumeratorAttributeChange(const FOnAttributeChangeData& CallbackData);
	void OnDenominatorAttributeChange(const FOnAttributeChangeData& CallbackData);

	/** Wait on attribute ratio change meeting a comparison threshold. */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAA_WaitAttributeChangeRatioThreshold* WaitForAttributeChangeRatioThreshold(AActor* TargetActor, FGameplayAttribute AttributeNumerator, FGameplayAttribute AttributeDenominator, TEnumAsByte<EWaitAttributeChangeComparison::Type> ComparisonType, float ComparisonValue, bool bTriggerOnce, AActor* OptionalExternalOwner = nullptr);

	FGameplayAttribute AttributeNumerator;
	FGameplayAttribute AttributeDenominator;
	TEnumAsByte<EWaitAttributeChangeComparison::Type> ComparisonType = EWaitAttributeChangeComparison::LessThanOrEqualTo;
	float ComparisonValue = 0.0f;
	bool bTriggerOnce;
	FDelegateHandle OnNumeratorAttributeChangeDelegateHandle;
	FDelegateHandle OnDenominatorAttributeChangeDelegateHandle;

protected:
	virtual void Activate() override;
	virtual void EndAction() override;

	float LastAttributeNumeratorValue;
	float LastAttributeDenominatorValue;
	bool bMatchedComparisonLastAttributeChange;

	/** Timer used when either numerator or denominator attribute is changed to delay checking of ratio to avoid false positives (MaxHealth changed before Health updates accordingly) */
	FTimerHandle CheckAttributeTimer;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ExternalOwner;

	UAbilitySystemComponent* GetFocusedASC() const;

	void OnAttributeChange();
	void OnRatioChange();

	bool DoesValuePassComparison(float ValueNumerator, float ValueDenominator) const;
	
};
