// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/AA_TryActivateAbilityAndWait.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "UHLGASBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Data/PayloadWithInstancedStructs.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AA_TryActivateAbilityAndWait)

void UAA_TryActivateAbilityAndWait::OnAbilityActivate(UGameplayAbility* ActivatedAbility)
{
	if (!IncludeTriggeredAbilities && ActivatedAbility->IsTriggered())
	{
		return;
	}

	const FGameplayTagContainer& AbilityTags = ActivatedAbility->GetAssetTags();
	if (WithTag.IsValid() && !AbilityTags.HasTag(WithTag))
	{
		// Failed tag check
		return;
	}

	if (ShouldBroadcastDelegates())
	{
		OnActivate.Broadcast(ActivatedAbility);
	}

	// if (TriggerOnce)
	// {
	// 	EndAction();
	// }
}

void UAA_TryActivateAbilityAndWait::OnAbilityDeactivate(UGameplayAbility* ActivatedAbility)
{
	const FGameplayTagContainer& AbilityTags = ActivatedAbility->GetAssetTags();
	if (WithTag.IsValid() && !AbilityTags.HasTag(WithTag))
	{
		// Failed tag check
		return;
	}

	if (ShouldBroadcastDelegates())
	{
		OnEndAbility.Broadcast(ActivatedAbility);
	}

	if (TriggerOnce)
	{
		EndAction();
	}
}

UAA_TryActivateAbilityAndWait* UAA_TryActivateAbilityAndWait::TryActivateAbilityAndWait(AActor* TargetActor,
	FGameplayTag InWithTag, bool bUsingEvent_In, const TArray<FInstancedStruct>& PayloadInfo, bool InIncludeTriggeredAbilities, bool bAllowRemoteActivation, bool InTriggerOnce)
{
	UAA_TryActivateAbilityAndWait* MyObj = NewObject<UAA_TryActivateAbilityAndWait>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->WithTag = InWithTag;
	MyObj->bUsingEvent = bUsingEvent_In;
	MyObj->IncludeTriggeredAbilities = InIncludeTriggeredAbilities;
	MyObj->TriggerOnce = InTriggerOnce;
	MyObj->bAllowRemoteActivation = bAllowRemoteActivation;
	MyObj->PayloadInfo = PayloadInfo;
	return MyObj;
}

// UAA_TryActivateAbilityAndWait* UAA_TryActivateAbilityAndWait::WaitForAbilityDeactivateWithTagRequirements(
// 	AActor* TargetActor, FGameplayTagRequirements TagRequirements, bool InIncludeTriggeredAbilities,
// 	bool InTriggerOnce)
// {
// 	UAA_TryActivateAbilityAndWait* MyObj = NewObject<UAA_TryActivateAbilityAndWait>();
// 	MyObj->SetAbilityActor(TargetActor);
// 	MyObj->TagRequirements = TagRequirements;
// 	MyObj->IncludeTriggeredAbilities = InIncludeTriggeredAbilities;
// 	MyObj->TriggerOnce = InTriggerOnce;
// 	return MyObj;
// }
//
// UAA_TryActivateAbilityAndWait* UAA_TryActivateAbilityAndWait::WaitForAbilityDeactivate_Query(
// 	AActor* TargetActor, FGameplayTagQuery Query, bool InIncludeTriggeredAbilities, bool InTriggerOnce)
// {
// 	UAA_TryActivateAbilityAndWait* MyObj = NewObject<UAA_TryActivateAbilityAndWait>();
// 	MyObj->SetAbilityActor(TargetActor);
// 	MyObj->Query = Query;
// 	MyObj->IncludeTriggeredAbilities = InIncludeTriggeredAbilities;
// 	MyObj->TriggerOnce = InTriggerOnce;
// 	return MyObj;
// }

void UAA_TryActivateAbilityAndWait::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		OnAbilityActivateDelegateHandle = ASC->AbilityActivatedCallbacks.AddUObject(this, &UAA_TryActivateAbilityAndWait::OnAbilityActivate);
		OnAbilityActivateDelegateHandle = ASC->AbilityEndedCallbacks.AddUObject(this, &UAA_TryActivateAbilityAndWait::OnAbilityDeactivate);

		bool bActivated = false;
		if (bUsingEvent)
		{
			UPayloadWithInstancedStructs* Payload = NewObject<UPayloadWithInstancedStructs>();
			Payload->InstancedStructs = PayloadInfo;
			FGameplayEventData EventData;
			EventData.OptionalObject = Payload;
			int32 ActivatedAbilitiesCount = ASC->HandleGameplayEvent(WithTag, &EventData);
			bActivated = (ActivatedAbilitiesCount > 0);
		}
		else
		{
			bActivated = UUHLGASBlueprintLibrary::TryActivateAbilityWithTag(ASC, WithTag, bAllowRemoteActivation);
		}
		if (!bActivated)
		{
			EndAction();
		}
	}
	else
	{
		EndAction();
	}
}

void UAA_TryActivateAbilityAndWait::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityActivatedCallbacks.Remove(OnAbilityActivateDelegateHandle);
	}
	Super::EndAction();
}

