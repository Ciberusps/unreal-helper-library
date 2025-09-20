// Pavel Penkov 2025 All Rights Reserved.


#include "Development/UHLDebugSystemSettings.h"

#include "Core/UHLGameplayTags.h"
#include "UHLDebugCategory.h"
#include "UHLDebugSystemSubsystem.h"
#include "DebugCategories/DCC_AbilitySystem_Abilities.h"
#include "DebugCategories/DCC_AbilitySystem_Attributes.h"
#include "DebugCategories/DCC_AbilitySystem_Effects.h"
#include "DebugCategories/DCC_Collisions.h"
#include "DebugCategories/DCC_InputSystem_EnhancedInput.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugSystemSettings)

void UUHLDebugSystemSettings::OnEnabledDebugCategoryEntryChanged(
	FGameplayTag ChangedDebugCategoryTag, bool bEnabled)
{
	if (ChangedDebugCategoryTag != FGameplayTag::EmptyTag)
	{
		FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
		{
			return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(ChangedDebugCategoryTag));
		});
		if (UHLDebugCategory != nullptr)
		{
            	
			for (FGameplayTag BlockedTag : UHLDebugCategory->Blocks.GetGameplayTagArray())
			{
				for (TTuple<FGameplayTag, bool>& EnabledDebugCategory : EnabledDebugCategories)
				{
					if (!EnabledDebugCategory.Key.MatchesTag(ChangedDebugCategoryTag)
						&& EnabledDebugCategory.Key.MatchesTag(BlockedTag))
					{
						EnabledDebugCategory.Value = false;
					}
				}
			}

			for (FGameplayTag RequiredDebugCategoryTag : UHLDebugCategory->RequiredDebugCategories.GetGameplayTagArray())
			{
				for (TTuple<FGameplayTag, bool>& EnabledDebugCategory : EnabledDebugCategories)
				{
					if (!EnabledDebugCategory.Key.MatchesTag(ChangedDebugCategoryTag)
						&& EnabledDebugCategory.Key.MatchesTag(RequiredDebugCategoryTag))
					{
						EnabledDebugCategory.Value = true;
					}
				}
			}
            	
			// UpdateEnabledDebugCategoriesList();
		}
	}
	
	EnabledDebugCategories[ChangedDebugCategoryTag] = bEnabled;
}

TArray<FUHLDebugCategory> UUHLDebugSystemSettings::GET_DEFAULT_UHL_DEBUG_CATEGORIES()
{
    TArray<FUHLDebugCategory> DEFAULT_UHL_DEBUG_CATEGORIES = {};

    FUHLDebugCategory AbilitySystemAttributesDebugCategory = {};
    AbilitySystemAttributesDebugCategory.Name = "AbilitySystem Attributes";
    AbilitySystemAttributesDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem_Attributes };
    AbilitySystemAttributesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    AbilitySystemAttributesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem);
    AbilitySystemAttributesDebugCategory.Components = { UDCC_AbilitySystem_Attributes::StaticClass() };
    AbilitySystemAttributesDebugCategory.bIsDefaultUHLDebugCategory = true;
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilitySystemAttributesDebugCategory);

    FUHLDebugCategory AbilitySystemEffectsDebugCategory = {};
    AbilitySystemEffectsDebugCategory.Name = "AbilitySystem Effects";
    AbilitySystemEffectsDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem_Effects };
    AbilitySystemEffectsDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    AbilitySystemEffectsDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem);
    AbilitySystemEffectsDebugCategory.Components = { UDCC_AbilitySystem_Effects::StaticClass() };
    AbilitySystemEffectsDebugCategory.bIsDefaultUHLDebugCategory = true;
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilitySystemEffectsDebugCategory);

    FUHLDebugCategory AbilitySystemAbilitiesDebugCategory = {};
    AbilitySystemAbilitiesDebugCategory.Name = "AbilitySystem Abilities";
    AbilitySystemAbilitiesDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem_Abilities };
    AbilitySystemAbilitiesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    AbilitySystemAbilitiesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem);
    AbilitySystemAbilitiesDebugCategory.Components = { UDCC_AbilitySystem_Abilities::StaticClass() };
    AbilitySystemAbilitiesDebugCategory.bIsDefaultUHLDebugCategory = true;
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilitySystemAbilitiesDebugCategory);

    FUHLDebugCategory EnhancedInputSystemDebugCategory = {};
    EnhancedInputSystemDebugCategory.Name = "EnhancedInputSystem";
    EnhancedInputSystemDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem };
    EnhancedInputSystemDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    EnhancedInputSystemDebugCategory.Components = { UDCC_InputSystem_EnhancedInput::StaticClass() };
    EnhancedInputSystemDebugCategory.bIsDefaultUHLDebugCategory = true;
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(EnhancedInputSystemDebugCategory);

    FUHLDebugCategory AbilityInputCacheDebugCategory = {};
    AbilityInputCacheDebugCategory.Name = "AbilityInputCache";
    AbilityInputCacheDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_AbilityInputCache };
    AbilityInputCacheDebugCategory.bIsDefaultUHLDebugCategory = true;
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilityInputCacheDebugCategory);

	FUHLDebugCategory CollisionsDebugCategory = {};
	CollisionsDebugCategory.Name = "Collisions";
	CollisionsDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_Collisions };
	EnhancedInputSystemDebugCategory.Components = { UDCC_Collisions::StaticClass() };
	CollisionsDebugCategory.bIsDefaultUHLDebugCategory = true;
	DEFAULT_UHL_DEBUG_CATEGORIES.Add(CollisionsDebugCategory);

    return DEFAULT_UHL_DEBUG_CATEGORIES;
}

#if WITH_EDITOR
void UUHLDebugSystemSettings::PostInitProperties()
{
    Super::PostInitProperties();

    UpdateDefaultUHLDebugCategories();

    for (const FUHLDebugCategory& DebugCategory : DebugCategories)
    {
        if (DebugCategory.ByDefaultEnabledInBuildTypes.Contains(UUnrealHelperLibraryBPL::GetBuildType()))
        {
            EnabledDebugCategories.Emplace(DebugCategory.Tags.First(), true);
        }
    }

    RecreateEnabledDebugCategoriesList();
}

void UUHLDebugSystemSettings::PreEditChange(FProperty* PropertyAboutToChange)
{
    if (PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSystemSettings, EnabledDebugCategories))
    {
        LastEnabledDebugCategories = TMap(EnabledDebugCategories);
    }

    Super::PreEditChange(PropertyAboutToChange);
}

void UUHLDebugSystemSettings::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
    FName PropertyName = PropertyChangedEvent.GetPropertyName();

    bool bEditingEnabledCategoriesKey = PropertyName == "EnabledDebugCategories_Key";
    if (bEditingEnabledCategoriesKey || (EnabledDebugCategories.IsEmpty() && !DebugCategories.IsEmpty()))
    {
        RecreateEnabledDebugCategoriesList();
        return;
    }

    Super::PostEditChangeChainProperty(PropertyChangedEvent);

    bool bEditingEnabledDebugCategories =
        PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSystemSettings, EnabledDebugCategories)
        && !LastEnabledDebugCategories.IsEmpty()
		&& !EnabledDebugCategories.IsEmpty()
        && LastEnabledDebugCategories.Num() == EnabledDebugCategories.Num();

    // disable other DebugCategories by "Blocks" tags
    if (bEditingEnabledDebugCategories)
    {
    	bool NewValue = false;
        FGameplayTag ChangedDebugCategoryTag = FGameplayTag::EmptyTag;
		for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : EnabledDebugCategories)
		{
			if (LastEnabledDebugCategories[EnabledDebugCategory.Key] != EnabledDebugCategory.Value)
			{
				NewValue = EnabledDebugCategory.Value;
				ChangedDebugCategoryTag = EnabledDebugCategory.Key;
			}
		}

    	OnEnabledDebugCategoryEntryChanged(ChangedDebugCategoryTag, NewValue);
    }

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSystemSettings, bExcludeDefaultUHLDebugCategories))
    {
        UpdateDefaultUHLDebugCategories();
        RecreateEnabledDebugCategoriesList();
    }

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSystemSettings, DebugCategories)
        || PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSystemSettings, DebugCategories))
    {
        RecreateEnabledDebugCategoriesList();
    }
}
#endif

void UUHLDebugSystemSettings::RecreateEnabledDebugCategoriesList()
{
    TMap<FGameplayTag, bool> CopyOfEnabledDebugCategories = TMap(EnabledDebugCategories);

    EnabledDebugCategories.Empty();
    for (const FUHLDebugCategory& DebugCategory : DebugCategories)
    {
        for (FGameplayTag GameplayTag : DebugCategory.Tags.GetGameplayTagArray())
        {
            bool* KeyVal = CopyOfEnabledDebugCategories.Find(GameplayTag);
            EnabledDebugCategories.Add(GameplayTag, KeyVal ? *KeyVal : false);
        }
    }
}

void UUHLDebugSystemSettings::UpdateEnabledDebugCategoriesList()
{
    for (const FUHLDebugCategory& DebugCategory : DebugCategories)
    {
        TArray<FGameplayTag> Keys;
        EnabledDebugCategories.GetKeys(Keys);

        FGameplayTagContainer KeysContainer = FGameplayTagContainer::CreateFromArray(Keys);
        if (!KeysContainer.HasAll(DebugCategory.Tags))
        {
            for (FGameplayTag GameplayTag : DebugCategory.Tags.GetGameplayTagArray())
            {
                if (GameplayTag.MatchesAny(KeysContainer))
                {
                    continue;
                }
                EnabledDebugCategories.Add(GameplayTag, false);
            }
        }
    }
}

void UUHLDebugSystemSettings::UpdateDefaultUHLDebugCategories()
{
    if (bExcludeDefaultUHLDebugCategories)
    {
        DebugCategories.RemoveAll([=](const FUHLDebugCategory& DebugCategory)
        {
            return DebugCategory.bIsDefaultUHLDebugCategory;
        });
    }
    else
    {
        for (const FUHLDebugCategory& DefaultUHLDebugCategory : GET_DEFAULT_UHL_DEBUG_CATEGORIES())
        {
            FUHLDebugCategory* FoundDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& Category)
            {
               return Category.Tags == DefaultUHLDebugCategory.Tags;
            });
            if (!FoundDebugCategory)
            {
                DebugCategories.Add(DefaultUHLDebugCategory);
            }
        }
    }
}
