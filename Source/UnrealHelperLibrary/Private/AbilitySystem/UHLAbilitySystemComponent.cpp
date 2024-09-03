// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/UHLAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/UHLGameplayAbility.h"
#include "Core/UHLGameplayTags.h"
#include "Utils/UnrealHelperLibraryBPL.h"

void UUHLAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

    AbilityInputCache = NewObject<UAbilityInputCache>(this);
    AbilityInputCache->SetUp(this);

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UUHLAbilitySystemComponent::InitAbilitySystem(TObjectPtr<AController> NewController, TObjectPtr<AActor> InAvatarActor)
{
	InitAbilityActorInfo(NewController, InAvatarActor);

    if (bGiveAbilitiesOnStart)
    {
		for (auto& Ability : Abilities)
		{
			GiveAbility(FGameplayAbilitySpec(Ability));
		}
    }
}

void UUHLAbilitySystemComponent::InitAbilitySystem(AActor* NewOwner, AActor* InAvatarActor)
{
	InitAbilityActorInfo(NewOwner, InAvatarActor);

    if (bGiveAbilitiesOnStart)
    {
        for (auto& Ability : Abilities)
        {
            GiveAbility(FGameplayAbilitySpec(Ability));
        }
    }
}

void UUHLAbilitySystemComponent::InitAttributes()
{
    if (bInitializeGameplayAttributes)
    {
	    SetAttributes(InitialGameplayAttributes);
    }
    if (bGiveInitialGameplayTags)
    {
        AddLooseGameplayTags(InitialGameplayTags, 1);
    }
}

void UUHLAbilitySystemComponent::SetAttributes(TMap<FGameplayAttribute, float> Attributes_In)
{
    for (TTuple<FGameplayAttribute, float> InitialAttributePair : Attributes_In)
    {
        if (!HasAttributeSetForAttribute(InitialAttributePair.Key)) continue;

        SetNumericAttributeBase(InitialAttributePair.Key, InitialAttributePair.Value);
    }
}

void UUHLAbilitySystemComponent::ActivateInitialAbilities()
{
    if (bActivateAbilitiesOnStart)
    {
        for (const auto AbilityTags : InitialActiveAbilities)
        {
            TryActivateAbilityWithTag(AbilityTags.First());
        }
    }
}

void UUHLAbilitySystemComponent::OnUnregister()
{
	FGameplayTagContainer AllTags;
	GetOwnedGameplayTags(AllTags);
	RemoveLooseGameplayTags(AllTags);

	Super::OnUnregister();
}

bool UUHLAbilitySystemComponent::TryActivateAbilityWithTag(FGameplayTag GameplayTag, bool bAllowRemoteActivation)
{
	return TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayTag), bAllowRemoteActivation);
}

bool UUHLAbilitySystemComponent::TryCancelAbilityWithTag(FGameplayTag GameplayTag)
{
	bool bResult = false;
	TArray<FGameplayAbilitySpec*> AbilitiesToCancel;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), AbilitiesToCancel, false);

	for (FGameplayAbilitySpec* AbilitySpec : AbilitiesToCancel)
	{
		TArray<UGameplayAbility*> AbilityInstances = AbilitySpec->GetAbilityInstances();
		for (UGameplayAbility* Ability : AbilityInstances)
		{
			if (Ability->IsActive())
			{
				Ability->K2_CancelAbility();
				bResult = true;
			}
		}
	}
	return bResult;
}

TArray<bool> UUHLAbilitySystemComponent::TryCancelAbilitiesWithTags(TArray<FGameplayTag> GameplayTags)
{
	TArray<bool> Result;
	for (auto GameplayTag : GameplayTags)
	{
		Result.Add(TryCancelAbilityWithTag(GameplayTag));
	}
	return Result;
}

int32 UUHLAbilitySystemComponent::FireGameplayEvent(FGameplayTag EventTag, const FGameplayEventData& Payload)
{
	return HandleGameplayEvent(EventTag, &Payload);
}

bool UUHLAbilitySystemComponent::CanAddAbilityToCache(UUHLGameplayAbility* GameplayAbility_In) const
{
    bool bHasRequiredTags = HasAllMatchingGameplayTags(GameplayAbility_In->AddingToCacheInputRequiredTags);
    bool bDontHaveBlockedTags = !HasAnyMatchingGameplayTags(GameplayAbility_In->AddingToCacheInputBlockedTags);

    return bHasRequiredTags && bDontHaveBlockedTags;
}

bool UUHLAbilitySystemComponent::IsAbilityActive(FGameplayTag GameplayTag) const
{
	bool bResult = false;
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* AbilitySpec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = AbilitySpec->GetAbilityInstances();
		for (UGameplayAbility* Ability : AbilityInstances)
		{
			bResult |= Ability->IsActive();
		}
	}
	return bResult;
}

void UUHLAbilitySystemComponent::RemoveLooseGameplayTagCompletly(const FGameplayTag& GameplayTag)
{
    RemoveLooseGameplayTag(GameplayTag, 999999);
}

void UUHLAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UUHLAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UUHLAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Replaced AbilitySpec.DynamicAbilityTags on AbilitySpec.Ability->AbilityTags
			if (AbilitySpec.Ability && (AbilitySpec.Ability->AbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UUHLAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Replaced AbilitySpec.DynamicAbilityTags on AbilitySpec.Ability->AbilityTags
			if (AbilitySpec.Ability && (AbilitySpec.Ability->AbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

// copy paste from LyraAbilitySystemComponent
void UUHLAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
    if (!bUseInputConfig) return;

	// TODO: mb check how Lyra use that tag?
	if (HasMatchingGameplayTag(UHLGameplayTags::TAG_Gameplay_AbilityInputBlocked))
	{
		InputPressedSpecHandles.Reset();
		InputReleasedSpecHandles.Reset();
		InputHeldSpecHandles.Reset();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UUHLGameplayAbility* AbilityCDO = CastChecked<UUHLGameplayAbility>(AbilitySpec->Ability);

				if (AbilityCDO->GetActivationPolicy() == EUHLAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				// TODO: если абилка активна, нужно пытаться все равно ее активировать, а не просто данные слать
				// If ability active, we should try to activate it again, instead of sending data

				// if (AbilitySpec->IsActive())
				// {
				// 	// Ability is active so pass along the input event.
				// 	AbilitySpecInputPressed(*AbilitySpec);
				// }
				// else
				// {
				const UUHLGameplayAbility* AbilityCDO = CastChecked<UUHLGameplayAbility>(AbilitySpec->Ability);

				if (AbilityCDO->GetActivationPolicy() == EUHLAbilityActivationPolicy::OnInputTriggered)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);

					// TODO: testing
					// if (AbilitySpec->IsActive())
					// {
						// Ability is active so pass along the input event.
						AbilitySpecInputPressed(*AbilitySpec);
					// }
				}
				// }
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
    // TODO AbiilityInputCache field of improvement - check not "bAtLeastOneCachedAbilityActivated"
    // but how much abilities was activated from "FUHLInputActionAbilities"
    // if at least one from "FUHLInputActionAbilities" activated skip adding others to cache
    bool bAtLeastOneCachedAbilityActivated = false;
    TArray<FGameplayTag> AbilityTagsRequiredToBeCached = {};
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
	    if (!AbilitySpecHandle.IsValid()) continue;

		bool bActivated = TryActivateAbility(AbilitySpecHandle);

	    // check AbilityCache
	    if (bUseAbilityInputCache && AbilitySpecHandle.IsValid())
	    {
	        // works if not using cache windows or use them and now in "CatchTo AbilityInputCache" window
            if (!bUseInputCacheWindows || (bUseInputCacheWindows && HasMatchingGameplayTag(UHLGameplayTags::TAG_UHL_AbilityInputCache_Catching)))
            {
				FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
				if (AbilitySpec->Ability)
				{
					UUHLGameplayAbility* GameplayAbility = StaticCast<UUHLGameplayAbility*>(AbilitySpec->Ability.Get());
					if (GameplayAbility->bCacheInput && !bActivated)
					{
						if (CanAddAbilityToCache(GameplayAbility))
						{
							AbilityTagsRequiredToBeCached.Add(GameplayAbility->AbilityTags.First());
						}
					}

					if (GameplayAbility->bCacheInput && bActivated)
					{
						bAtLeastOneCachedAbilityActivated = true;
						UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("At least one activated %s"), *GameplayAbility->AbilityTags.First().ToString()));
					}
				}
			}
	    }
	}

    if (!bAtLeastOneCachedAbilityActivated)
    {
        AbilityInputCache->AddTagsToCache(AbilityTagsRequiredToBeCached);
    }

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
