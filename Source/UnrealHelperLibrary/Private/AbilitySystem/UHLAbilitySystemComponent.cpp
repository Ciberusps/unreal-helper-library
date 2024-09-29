// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/UHLAbilitySystemComponent.h"

#include "AbilitySystem/UHLAbilitySet.h"
#include "AbilitySystem/Abilities/UHLGameplayAbility.h"
#include "Core/UHLGameplayTags.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAbilitySystemComponent)

void UUHLAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

    AbilityInputCache = NewObject<UAbilityInputCache>(this);
    AbilityInputCache->SetUp(this);

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
    
	AbilitySetGrantedHandles.Reset();
}

void UUHLAbilitySystemComponent::InitAbilitySystem(AActor* NewOwner, AActor* InAvatarActor, bool bActivateInitialAbilities)
{
	InitAbilityActorInfo(NewOwner, InAvatarActor);

    if (bGiveAbilitiesOnStart)
    {
        for (auto& Ability : Abilities)
        {
            GiveAbility(FGameplayAbilitySpec(Ability));
        }
    }
    if (bGiveAbilitySetsOnStart)
    {
        for (const TObjectPtr<const UUHLAbilitySet>& AbilitySet : AbilitySets)
        {
            GiveAbilitySet(AbilitySet);
        }
    }
    if (bGiveAttributesSetsOnStart)
    {
        for (TSubclassOf<UAttributeSet> AttributeSet : AttributeSets)
        {
            UAttributeSet* NewSet = NewObject<UAttributeSet>(GetOwner(), AttributeSet);
            AddAttributeSetSubobject(NewSet);
        }
    }

    InitAttributes();

    if (bActivateInitialAbilities)
    {
        ActivateInitialAbilities();
    }
}

void UUHLAbilitySystemComponent::InitAttributes()
{
    if (bInitializeGameplayAttributes)
    {
	    SetAttributes(InitialAttributes);
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
        for (const auto AbilityTags : ActiveAbilitiesOnStart)
        {
            TryActivateAbilityWithTag(AbilityTags.First());
        }
    }
}

void UUHLAbilitySystemComponent::GiveAbilitySet(const UUHLAbilitySet* AbilitySet)
{
    FUHLAbilitySet_GrantedHandles OutGrantedHandles;
    AbilitySet->GiveToAbilitySystem(this, &OutGrantedHandles, this);
    AbilitySetGrantedHandles.Add(OutGrantedHandles);
}

void UUHLAbilitySystemComponent::RemoveAbilitySetByTag(const FGameplayTag& GameplayTag)
{
    for (int32 i = 0; i < AbilitySetGrantedHandles.Num(); i++)
    {
        FUHLAbilitySet_GrantedHandles& AbilitySetGrantedHandle = AbilitySetGrantedHandles[i];
        if (AbilitySetGrantedHandle.GetAbilitySetTags().HasAny(FGameplayTagContainer(GameplayTag)))
        {
            AbilitySetGrantedHandle.TakeFromAbilitySystem(this);
            AbilitySetGrantedHandles.RemoveAt(i, 1);
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

#if WITH_EDITOR
void UUHLAbilitySystemComponent::PostInitProperties()
{
    Super::PostInitProperties();

    UpdatePreviewAbilitiesMap();
}

void UUHLAbilitySystemComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, AbilitySets)
        || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, DebugPreviewAbilitiesFromAbilitySets))
    {
        UpdatePreviewAbilitiesMap();
    }
}

void UUHLAbilitySystemComponent::UpdatePreviewAbilitiesMap()
{
    DebugPreviewAbilitiesFromAbilitySets.Reset();

    if (!bPreviewAllAbilities)
    {
        return;
    }

    for (const UUHLAbilitySet* AbilitySet : AbilitySets)
    {
        if (!AbilitySet) continue;

        TTuple<FString, FString> TuplePreview;
        TuplePreview.Key = AbilitySet->GetName();

        for (const TSubclassOf<UGameplayAbility>& AbilityRef : AbilitySet->GetAllAbilitiesList())
        {
            if (!AbilityRef.Get()) continue; 
            TuplePreview.Value += AbilityRef->GetName().Replace(TEXT("_C"), TEXT("")) + "\n";
        }
        DebugPreviewAbilitiesFromAbilitySets.Add(TuplePreview);
    }
}
#endif

bool UUHLAbilitySystemComponent::TryActivateAbilityWithTag(FGameplayTag GameplayTag, bool bAllowRemoteActivation)
{
	return UUnrealHelperLibraryBPL::TryActivateAbilityWithTag(this, GameplayTag, bAllowRemoteActivation);
}

bool UUHLAbilitySystemComponent::TryCancelAbilityWithTag(FGameplayTag GameplayTag)
{
    return UUnrealHelperLibraryBPL::TryCancelAbilityWithTag(this, GameplayTag);
}

TArray<bool> UUHLAbilitySystemComponent::TryCancelAbilitiesWithTags(TArray<FGameplayTag> GameplayTags)
{
    return UUnrealHelperLibraryBPL::TryCancelAbilitiesWithTags(this, GameplayTags);
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

void UUHLAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag InputTag)
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

void UUHLAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag InputTag)
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
