// Pavel Penkov 2025 All Rights Reserved.


#include "UHLAbilitySystemComponent.h"

#include "UHLAbilitySet.h"
#include "UHLGASBlueprintLibrary.h"
#include "Abilities/UHLGameplayAbility.h"
#include "Core/UHLGameplayTags.h"
#include "Development/UHLGASSettings.h"
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

	// AbilityActivatedCallbacks.AddUObject(this, )
}

UUHLAbilitySystemComponent::UUHLAbilitySystemComponent()
{
	const UUHLGASSettings* UHLSettings = GetDefault<UUHLGASSettings>();
	if (UHLSettings->bUseAbilitySystemConfigDefaultsInASC)
	{
		FillSettingsFromConfig(UHLSettings->AbilitySystemConfigDefaults);
	}
}

void UUHLAbilitySystemComponent::InitAbilitySystem(AActor* NewOwner, AActor* InAvatarActor, bool bActivateInitialAbilities)
{
	if (bUseAbilitySystemConfig && AbilitySystemConfig)
	{
		FillSettingsFromConfig(AbilitySystemConfig->Settings);
	}
	
	InitAbilityActorInfo(NewOwner, InAvatarActor);

	if (bGiveAttributesSetsOnStart)
	{
		for (TSubclassOf<UAttributeSet> AttributeSet : AttributeSets)
		{
			UAttributeSet* NewSet = NewObject<UAttributeSet>(GetOwner(), AttributeSet);
			AddAttributeSetSubobject(NewSet);
		}
	}

	InitAttributes();
	GiveInitialTags();
	
    if (bGiveAbilitiesOnStart)
    {
        for (auto& Ability : Abilities)
        {
            GiveAbility(FGameplayAbilitySpec(Ability));
        }
    }

	// TODO:
	// What if I want to give AbilitySet that initialized with my attributes?
	// Move attributes initialize to AbilitySet? Already can be done through GameplayEffect?
	// Try to initialize only attributes from AttributeSet? and after abilities/gameplayeffects?
    if (bGiveAbilitySetsOnStart)
    {
        for (const TObjectPtr<const UUHLAbilitySet>& AbilitySet : AbilitySets)
        {
            GiveAbilitySet(AbilitySet);
        }
    }

    if (bActivateInitialAbilities)
    {
        ActivateInitialAbilities();
    }
}

void UUHLAbilitySystemComponent::InitAttributes_Implementation()
{
    if (bInitializeGameplayAttributes)
    {
        SetAttributes(InitialAttributes);
    }
}

void UUHLAbilitySystemComponent::GiveInitialTags()
{
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

bool UUHLAbilitySystemComponent::CanEditChange(const FProperty* InProperty) const
{
	const bool ParentVal = Super::CanEditChange(InProperty);

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bInitializeGameplayAttributes)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, InitialAttributes)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bGiveAbilitiesOnStart)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, Abilities)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bGiveAttributesSetsOnStart)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, AttributeSets)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bActivateAbilitiesOnStart)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, ActiveAbilitiesOnStart)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bGiveInitialGameplayTags)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, InitialGameplayTags)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bGiveAbilitySetsOnStart)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, AbilitySets)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bPreviewAllAbilities)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, DebugPreviewAbilitiesFromAbilitySets)

		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bUseInputConfig)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, InputConfig)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bUseAbilityInputCache)
		|| InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UUHLAbilitySystemComponent, bUseInputCacheWindows)
	)
	{
		return !bUseAbilitySystemConfig;
	}

	return ParentVal;
}

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

void UUHLAbilitySystemComponent::FillSettingsFromConfig(const FUHLAbilitySystemSettings& AbilitySystemConfig_In)
{
	bInitializeGameplayAttributes = AbilitySystemConfig_In.bInitializeGameplayAttributes;
	InitialAttributes = AbilitySystemConfig_In.InitialAttributes;

	bGiveAbilitiesOnStart = AbilitySystemConfig_In.bGiveAbilitiesOnStart;
	Abilities = AbilitySystemConfig_In.Abilities;

	bGiveAttributesSetsOnStart = AbilitySystemConfig_In.bGiveAttributesSetsOnStart;
	AttributeSets = AbilitySystemConfig_In.AttributeSets;

	bActivateAbilitiesOnStart = AbilitySystemConfig_In.bActivateAbilitiesOnStart;
	ActiveAbilitiesOnStart = AbilitySystemConfig_In.ActiveAbilitiesOnStart;

	bGiveInitialGameplayTags = AbilitySystemConfig_In.bGiveInitialGameplayTags;
	InitialGameplayTags = AbilitySystemConfig_In.InitialGameplayTags;
	
	bGiveAbilitySetsOnStart = AbilitySystemConfig_In.bGiveAbilitySetsOnStart;
	AbilitySets = AbilitySystemConfig_In.AbilitySets;

	// bPreviewAllAbilities = AbilitySystemConfig_In.bPreviewAllAbilities;
	// DebugPreviewAbilitiesFromAbilitySets = AbilitySystemConfig_In.DebugPreviewAbilitiesFromAbilitySets;

	bUseInputConfig = AbilitySystemConfig_In.bUseInputConfig;
	InputConfig = AbilitySystemConfig_In.InputConfig;
	bUseAbilityInputCache = AbilitySystemConfig_In.bUseAbilityInputCache;
	bUseInputCacheWindows = AbilitySystemConfig_In.bUseInputCacheWindows;
}

bool UUHLAbilitySystemComponent::TryActivateAbilityWithTag(FGameplayTag GameplayTag, bool bAllowRemoteActivation)
{
	return UUHLGASBlueprintLibrary::TryActivateAbilityWithTag(this, GameplayTag, bAllowRemoteActivation);
}

bool UUHLAbilitySystemComponent::TryCancelAbilityWithTag(FGameplayTag GameplayTag)
{
    return UUHLGASBlueprintLibrary::TryCancelAbilityWithTag(this, GameplayTag);
}

TArray<bool> UUHLAbilitySystemComponent::TryCancelAbilitiesWithTags(TArray<FGameplayTag> GameplayTags)
{
    return UUHLGASBlueprintLibrary::TryCancelAbilitiesWithTags(this, GameplayTags);
}

int32 UUHLAbilitySystemComponent::FireGameplayEvent(FGameplayTag EventTag, const FGameplayEventData& Payload)
{
	return HandleGameplayEvent(EventTag, &Payload);
}

bool UUHLAbilitySystemComponent::CanAddAbilityToCache(UUHLGameplayAbility* GameplayAbility_In) const
{
    bool bHasRequiredTags = HasAllMatchingGameplayTags(GameplayAbility_In->AddToCacheRequiredTags);
    bool bDontHaveBlockedTags = !HasAnyMatchingGameplayTags(GameplayAbility_In->AddToCacheBlockedTags);

    return bHasRequiredTags && bDontHaveBlockedTags;
}

bool UUHLAbilitySystemComponent::IsAbilityActive(FGameplayTag GameplayTag) const
{
	return UUHLGASBlueprintLibrary::IsAbilityActiveByTag(this, GameplayTag);
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
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
}

void UUHLAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}

void UUHLAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Replaced AbilitySpec.DynamicAbilityTags on AbilitySpec.Ability->GetAssetTags()
			if (AbilitySpec.Ability && (AbilitySpec.Ability->GetAssetTags().HasTagExact(InputTag)))
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
			if (AbilitySpec.Ability && (AbilitySpec.Ability->GetAssetTags().HasTagExact(InputTag)))
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

	if (HasMatchingGameplayTag(UHLGameplayTags::TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
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
				const UUHLGameplayAbility* AbilityCDO = Cast<UUHLGameplayAbility>(AbilitySpec->Ability);
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

				const UUHLGameplayAbility* AbilityCDO = Cast<UUHLGameplayAbility>(AbilitySpec->Ability);
				if (AbilitySpec->IsActive()
					// TODO move this logic to "OnInputTriggeredForceReactivate" ??
					// If ability active, we should try to activate it again, instead of sending data
					// so that's why if "OnInputTriggered" choosed - skip
					&& AbilityCDO
					&& AbilityCDO->GetActivationPolicy() != EUHLAbilityActivationPolicy::OnInputTriggered)
				{
                    // Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
				    // const UUHLGameplayAbility* AbilityCDO = Cast<UUHLGameplayAbility>(AbilitySpec->Ability);
                    if (AbilityCDO && AbilityCDO->GetActivationPolicy() == EUHLAbilityActivationPolicy::OnInputTriggered)
                    {
                        AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
                    }
                }
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
					if (GameplayAbility->bCacheInput && (!bActivated || GameplayAbility->bIgnoreActivatedState))
					{
						if (CanAddAbilityToCache(GameplayAbility))
						{
							AbilityTagsRequiredToBeCached.Add(GameplayAbility->GetAssetTags().First());
						}
					}

					if (GameplayAbility->bCacheInput && bActivated)
					{
						bAtLeastOneCachedAbilityActivated = true;
						UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("At least one activated %s"), *GameplayAbility->GetAssetTags().First().ToString()));
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
					
					// if "WhileInputActive" EndAbility automatically
					// const UUHLGameplayAbility* AbilityCDO = Cast<UUHLGameplayAbility>(AbilitySpec->Ability);
					// if (AbilityCDO && AbilityCDO->GetActivationPolicy() == EUHLAbilityActivationPolicy::WhileInputActive)
					// {
					// 	const FUHLWhileInputActiveSettings& WhileInputActiveSettings = AbilityCDO->GetWhileInputActiveSettings();
					// 	if (WhileInputActiveSettings.bCancelAbilityAutomatically)
					// 	{
					// 		// "EndAbility" not accessible, so try to cancel if "bCancelAbilityAutomatically"
					// 		AbilitySpec->Ability->CancelAbility(AbilitySpec->Handle, AbilityActorInfo.Get(), AbilitySpec->ActivationInfo, WhileInputActiveSettings.bReplicateEndAbility);
					// 		// AbilitySpec->Ability->EndAbility(AbilitySpec->Handle, AbilityActorInfo.Get(), AbilitySpec->ActivationInfo, WhileInputActiveSettings.bReplicateEndAbility, WhileInputActiveSettings.bMarkAsCanceledOnEnd);
					// 	}
					// }
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

void UUHLAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}
