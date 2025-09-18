// Pavel Penkov 2025 All Rights Reserved.




#include "Tasks/AA_TryActivateAbilityAndWait.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AA_TryActivateAbilityAndWait)

void UAA_TryActivateAbilityAndWait::OnAbilityActivate(UGameplayAbility* ActivatedAbility)
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (TrackedAbilitySpecHandle.IsValid())
		{
			if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(TrackedAbilitySpecHandle))
			{
				if (Spec->GetPrimaryInstance() == ActivatedAbility)
				{
					if (ShouldBroadcastDelegates())
					{
						OnActivate.Broadcast(ActivatedAbility);
					}	
				}
			}
		}
	}
}

void UAA_TryActivateAbilityAndWait::OnAbilityDeactivate(UGameplayAbility* ActivatedAbility)
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (TrackedAbilitySpecHandle.IsValid())
		{
			if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(TrackedAbilitySpecHandle))
			{
				if (Spec->GetPrimaryInstance() == ActivatedAbility)
				{
					if (ShouldBroadcastDelegates())
					{
						OnEndAbility.Broadcast(ActivatedAbility);
						EndAction();
					}	
				}
			}
		}
	}
}


UAA_TryActivateAbilityAndWait* UAA_TryActivateAbilityAndWait::TryActivateAbilityAndWait(AActor* TargetActor, FGameplayTagQuery TagQuery, bool bAllowRemoteActivation)
{
	UAA_TryActivateAbilityAndWait* MyObj = NewObject<UAA_TryActivateAbilityAndWait>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->TagQuery = TagQuery;
	MyObj->bAllowRemoteActivation = bAllowRemoteActivation;
	return MyObj;
}

void UAA_TryActivateAbilityAndWait::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		//@TODO: Оба делегата были в одном хендле, мб вернуть?
		OnAbilityActivateDelegateHandle = ASC->AbilityActivatedCallbacks.AddUObject(this, &UAA_TryActivateAbilityAndWait::OnAbilityActivate);
		OnAbilityDeactivateDelegateHandle = ASC->AbilityEndedCallbacks.AddUObject(this, &UAA_TryActivateAbilityAndWait::OnAbilityDeactivate);

		bool bActivated = false;
		{
			TArray<FGameplayAbilitySpecHandle> OutAbilityHandles;
			ASC->FindAllAbilitiesMatchingQuery(OutAbilityHandles, TagQuery);

			if (OutAbilityHandles.Num() > 0)
			{
				int32 Index = 0;
				while (Index < OutAbilityHandles.Num() && !bActivated)
				{
					FGameplayAbilitySpecHandle AbilitySpecHandle = OutAbilityHandles[Index];
					if (AbilitySpecHandle.IsValid())
					{
						TrackedAbilitySpecHandle = AbilitySpecHandle;  
						bActivated = ASC->TryActivateAbility(TrackedAbilitySpecHandle, bAllowRemoteActivation);
					}
					Index++;
				}
			}
		}
		if (!bActivated)
		{
			//If ability dint activated
			if (ShouldBroadcastDelegates())
			{
				OnActivationFailed.Broadcast();
			}
			EndAction();
		}
	}
	else
	{
		//If ability dint activated
		if (ShouldBroadcastDelegates())
		{
			OnActivationFailed.Broadcast();
		}
		EndAction();
	}
}

void UAA_TryActivateAbilityAndWait::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityActivatedCallbacks.Remove(OnAbilityActivateDelegateHandle);
		ASC->AbilityActivatedCallbacks.Remove(OnAbilityDeactivateDelegateHandle);
	}
	Super::EndAction();
}



