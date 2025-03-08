// Pavel Penkov 2025 All Rights Reserved.


#include "AbilitySystem/Tasks/AA_WaitAbilityActivate.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AA_WaitAbilityActivate)

void UAA_WaitAbilityActivate::OnAbilityActivate(UGameplayAbility* ActivatedAbility)
{
	if (!IncludeTriggeredAbilities && ActivatedAbility->IsTriggered())
	{
		return;
	}

	const FGameplayTagContainer& AbilityTags = ActivatedAbility->GetAssetTags();

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
		OnActivate.Broadcast(ActivatedAbility);
	}

	if (TriggerOnce)
	{
		EndAction();
	}
}

UAA_WaitAbilityActivate* UAA_WaitAbilityActivate::WaitForAbilityActivate(AActor* TargetActor,
                                                                      FGameplayTag InWithTag, FGameplayTag InWithoutTag, bool InIncludeTriggeredAbilities, bool InTriggerOnce)
{
	UAA_WaitAbilityActivate* MyObj = NewObject<UAA_WaitAbilityActivate>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->WithTag = InWithTag;
	MyObj->WithoutTag = InWithoutTag;
	MyObj->IncludeTriggeredAbilities = InIncludeTriggeredAbilities;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

UAA_WaitAbilityActivate* UAA_WaitAbilityActivate::WaitForAbilityActivateWithTagRequirements(
	AActor* TargetActor, FGameplayTagRequirements TagRequirements, bool InIncludeTriggeredAbilities,
	bool InTriggerOnce)
{
	UAA_WaitAbilityActivate* MyObj = NewObject<UAA_WaitAbilityActivate>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->TagRequirements = TagRequirements;
	MyObj->IncludeTriggeredAbilities = InIncludeTriggeredAbilities;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

UAA_WaitAbilityActivate* UAA_WaitAbilityActivate::WaitForAbilityActivate_Query(
	AActor* TargetActor, FGameplayTagQuery Query, bool InIncludeTriggeredAbilities, bool InTriggerOnce)
{
	UAA_WaitAbilityActivate* MyObj = NewObject<UAA_WaitAbilityActivate>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Query = Query;
	MyObj->IncludeTriggeredAbilities = InIncludeTriggeredAbilities;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void UAA_WaitAbilityActivate::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		OnAbilityActivateDelegateHandle = ASC->AbilityActivatedCallbacks.AddUObject(this, &UAA_WaitAbilityActivate::OnAbilityActivate);
	}
	else
	{
		EndAction();
	}
}

void UAA_WaitAbilityActivate::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityActivatedCallbacks.Remove(OnAbilityActivateDelegateHandle);
	}
	Super::EndAction();
}

