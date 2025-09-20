// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/AA_WaitAbilityDeactivate.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AA_WaitAbilityDeactivate)

void UAA_WaitAbilityDeactivate::OnAbilityDeactivate(UGameplayAbility* DeactivatedAbility)
{
	const FGameplayTagContainer& AbilityTags = DeactivatedAbility->GetAssetTags();

	if (TagRequirements.IsEmpty())
	{
		if ((WithTag.IsValid() && !AbilityTags.HasTag(WithTag)) ||
			(WithoutTag.IsValid() && AbilityTags.HasTag(WithoutTag)))
		{
			// Failed tag check
			return;
		}
	}
	else
	{
		if (!TagRequirements.RequirementsMet(AbilityTags))
		{
			// Failed tag check
			return;
		}
	}

	if (Query.IsEmpty() == false)
	{
		if (Query.Matches(AbilityTags) == false)
		{
			// Failed query
			return;
		}
	}

	if (ShouldBroadcastDelegates())
	{
		OnDeactivate.Broadcast(DeactivatedAbility);
	}

	if (TriggerOnce)
	{
		EndAction();
	}
}

UAA_WaitAbilityDeactivate* UAA_WaitAbilityDeactivate::WaitForAbilityDeactivate(AActor* TargetActor,
	FGameplayTag InWithTag, FGameplayTag InWithoutTag, bool InTriggerOnce)
{
	UAA_WaitAbilityDeactivate* MyObj = NewObject<UAA_WaitAbilityDeactivate>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->WithTag = InWithTag;
	MyObj->WithoutTag = InWithoutTag;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

UAA_WaitAbilityDeactivate* UAA_WaitAbilityDeactivate::WaitForAbilityDeactivateWithTagRequirements(
	AActor* TargetActor, FGameplayTagRequirements TagRequirements, bool InTriggerOnce)
{
	UAA_WaitAbilityDeactivate* MyObj = NewObject<UAA_WaitAbilityDeactivate>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->TagRequirements = TagRequirements;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

UAA_WaitAbilityDeactivate* UAA_WaitAbilityDeactivate::WaitForAbilityDeactivate_Query(
	AActor* TargetActor, FGameplayTagQuery Query, bool InTriggerOnce)
{
	UAA_WaitAbilityDeactivate* MyObj = NewObject<UAA_WaitAbilityDeactivate>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Query = Query;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void UAA_WaitAbilityDeactivate::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		OnAbilityDeactivateDelegateHandle = ASC->AbilityEndedCallbacks.AddUObject(this, &UAA_WaitAbilityDeactivate::OnAbilityDeactivate);
	}
	else
	{
		EndAction();
	}
}

void UAA_WaitAbilityDeactivate::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityEndedCallbacks.Remove(OnAbilityDeactivateDelegateHandle);
	}
	Super::EndAction();
}

