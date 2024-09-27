// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/UHLDebugSubsystemSettings.h"

#include "Core/UHLGameplayTags.h"
#include "Subsystems/DebugSubsystem/UHLDebugCategory.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Abilities.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Attributes.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Effects.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_InputSystem_EnhancedInput.h"
#include "Utils/UnrealHelperLibraryBPL.h"


TArray<FUHLDebugCategory> UUHLDebugSubsystemSettings::GET_DEFAULT_UHL_DEBUG_CATEGORIES()
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

    return DEFAULT_UHL_DEBUG_CATEGORIES;
}

FName UUHLDebugSubsystemSettings::GetCategoryName() const
{
    return FApp::GetProjectName();
}

#if WITH_EDITOR
void UUHLDebugSubsystemSettings::PostInitProperties()
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

void UUHLDebugSubsystemSettings::PreEditChange(FProperty* PropertyAboutToChange)
{
    if (PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategories))
    {
        LastEnabledDebugCategories = TMap(EnabledDebugCategories);
    }

    Super::PreEditChange(PropertyAboutToChange);
}

void UUHLDebugSubsystemSettings::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
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
        PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategories)
        && !LastEnabledDebugCategories.IsEmpty()
		&& !EnabledDebugCategories.IsEmpty()
        && LastEnabledDebugCategories.Num() == EnabledDebugCategories.Num();

    // disable other DebugCategories by "Blocks" tags
    if (bEditingEnabledDebugCategories)
    {
        FGameplayTag ChangedDebugCategoryTag = FGameplayTag::EmptyTag;
		for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : EnabledDebugCategories)
		{
			if (LastEnabledDebugCategories[EnabledDebugCategory.Key] != EnabledDebugCategory.Value
				&& EnabledDebugCategory.Value == true)
			{
				ChangedDebugCategoryTag = EnabledDebugCategory.Key;
			}
		}

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
                // UpdateEnabledDebugCategoriesList();
            }
        }
    }

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, bExcludeDefaultUHLDebugCategories))
    {
        UpdateDefaultUHLDebugCategories();
        RecreateEnabledDebugCategoriesList();
    }

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, DebugCategories)
        || PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, DebugCategories))
    {
        RecreateEnabledDebugCategoriesList();
    }
}
#endif

void UUHLDebugSubsystemSettings::RecreateEnabledDebugCategoriesList()
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

void UUHLDebugSubsystemSettings::UpdateEnabledDebugCategoriesList()
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

void UUHLDebugSubsystemSettings::UpdateDefaultUHLDebugCategories()
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
