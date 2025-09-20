// Pavel Penkov 2025 All Rights Reserved.

#include "UHLGASBlueprintLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Core/UHLAbilitySystemInterface.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UI/UHLHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLGASBlueprintLibrary)

UUHLAbilitySystemComponent* UUHLGASBlueprintLibrary::GetUHLAbilitySystemComponent(AActor* Actor)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}

	const IUHLAbilitySystemInterface* ASI = Cast<IUHLAbilitySystemInterface>(Actor);
	if (ASI)
	{
		return IUHLAbilitySystemInterface::Execute_GetUHLAbilitySystemComponent(Actor);
	}

	return Actor->FindComponentByClass<UUHLAbilitySystemComponent>();
}

FGameplayEffectSpec UUHLGASBlueprintLibrary::CreateGenericGASGameplayEffectSpec(
	TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* HitInstigator, AActor* InEffectCauser, const FHitResult& HitResult, const UObject* SourceObject)
{
	const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	FGameplayEffectContext* GameplayEffectContext = new FGameplayEffectContext(HitInstigator, InEffectCauser);
	FGameplayEffectContextHandle GameplayEffectContextHandle(GameplayEffectContext);
	GameplayEffectContextHandle.AddHitResult(HitResult);
	GameplayEffectContextHandle.AddSourceObject(SourceObject);
	FGameplayEffectSpec GameplayEffectSpec(GameplayEffect, GameplayEffectContextHandle);
	return GameplayEffectSpec;
}

void UUHLGASBlueprintLibrary::UpdateStateGameplayTags(UAbilitySystemComponent* ASC, bool bCondition, FGameplayTag PositiveConditionTag, FGameplayTag NegativeConditionTag, bool bShouldReplicate)
{
	if (!ASC)
	{
		return;
	}

	if (bCondition)
	{
		if (!ASC->HasMatchingGameplayTag(PositiveConditionTag))
		{
			if (bShouldReplicate)
			{
				ASC->AddReplicatedLooseGameplayTag(PositiveConditionTag);
			}
			else
			{
				ASC->AddLooseGameplayTag(PositiveConditionTag);
			}
		}
		if (NegativeConditionTag != FGameplayTag::EmptyTag)
		{
			if (bShouldReplicate)
			{
				ASC->RemoveReplicatedLooseGameplayTag(PositiveConditionTag);
			}
			else
			{
				ASC->RemoveLooseGameplayTag(NegativeConditionTag, 999999);
			}
		}
	}
	else
	{
		if (NegativeConditionTag == FGameplayTag::EmptyTag)
		{
			if (bShouldReplicate)
			{
				ASC->RemoveReplicatedLooseGameplayTag(PositiveConditionTag);	
			}
			else
			{
				ASC->RemoveLooseGameplayTag(PositiveConditionTag, 999999);	
			}
		}
		else
		{
			if (!ASC->HasMatchingGameplayTag(NegativeConditionTag))
			{
				if (bShouldReplicate)
				{
					ASC->AddReplicatedLooseGameplayTag(NegativeConditionTag);
				}
				else
				{
					ASC->AddLooseGameplayTag(NegativeConditionTag);
				}
			}
			if (bShouldReplicate)
			{
				ASC->RemoveReplicatedLooseGameplayTag(PositiveConditionTag);
			}
			else
			{
				ASC->RemoveLooseGameplayTag(PositiveConditionTag, 999999);
			}
		}
	}
}

bool UUHLGASBlueprintLibrary::IsAbilityActiveByTag(const UAbilitySystemComponent* ASC, FGameplayTag GameplayTag)
{
	if (!IsValid(ASC)) return false;

	bool bResult = false;
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), AbilitiesToActivate, false);

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

bool UUHLGASBlueprintLibrary::TryActivateAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag, bool bAllowRemoteActivation)
{
	if (!IsValid(ASC)) return false;
	return ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayTag), bAllowRemoteActivation);
}

bool UUHLGASBlueprintLibrary::TryCancelAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag)
{
	if (!IsValid(ASC))
		return false;

	bool bResult = false;
	TArray<FGameplayAbilitySpec*> AbilitiesToCancel;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), AbilitiesToCancel, false);

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

bool UUHLGASBlueprintLibrary::ToggleAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag, bool bAllowRemoteActivation)
{
	if (!IsValid(ASC)) return false; 

	bool bResult = false;
	
	if (IsAbilityActiveByTag(ASC, GameplayTag))
	{
		bResult = TryCancelAbilityWithTag(ASC, GameplayTag);
	}
	else
	{
		bResult = TryActivateAbilityWithTag(ASC, GameplayTag, bAllowRemoteActivation);
	}

	return bResult;
}

TArray<bool> UUHLGASBlueprintLibrary::TryCancelAbilitiesWithTags(UAbilitySystemComponent* ASC, TArray<FGameplayTag> GameplayTags)
{
	if (!IsValid(ASC))
		return {};

	TArray<bool> Result;
	for (auto GameplayTag : GameplayTags)
	{
		Result.Add(TryCancelAbilityWithTag(ASC, GameplayTag));
	}
	return Result;
}

int32 UUHLGASBlueprintLibrary::FireGameplayEvent(UAbilitySystemComponent* ASC, FGameplayTag EventTag, const FGameplayEventData& Payload) { return ASC->HandleGameplayEvent(EventTag, &Payload); }

FGameplayTag UUHLGASBlueprintLibrary::FindTagByString(const FString& TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag& TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				// UE_LOG(LogUnrealHelperLibrary, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

void UUHLGASBlueprintLibrary::FindAbilitySpecByTag(
	const TArray<FGameplayAbilitySpec>& AbilitiesSpecsContainer, FGameplayTag TagToFind, FGameplayAbilitySpec& AbilitySpec_Out)
{
	// Iterate all specs provided
	for (const FGameplayAbilitySpec& Spec : AbilitiesSpecsContainer)
	{
		// Check the static tags on the ability class
		const UGameplayAbility* AbilityCDO = Spec.GetPrimaryInstance() 
			? Spec.GetPrimaryInstance() 
			: Spec.Ability.Get();

		if (AbilityCDO)
		{
			// combine ability‐level and dynamic tags
			const FGameplayTagContainer& StaticTags = AbilityCDO->GetAssetTags();
			const FGameplayTagContainer& DynamicTags = Spec.GetDynamicSpecSourceTags();

			// If either has our desired tag, return this spec
			if (StaticTags.HasTag(TagToFind) || DynamicTags.HasTag(TagToFind))
			{
				AbilitySpec_Out = Spec;
				return;
			}
		}
	}

	// Not found
	return;
}

void UUHLGASBlueprintLibrary::FindAbilitySpecByTagUsingASC(
	UAbilitySystemComponent* ASC,
	FGameplayTag TagToFind, FGameplayAbilitySpec& AbilitySpec_Out)
{
	FindAbilitySpecByTag(ASC->GetActivatableAbilities(), TagToFind, AbilitySpec_Out);
}
