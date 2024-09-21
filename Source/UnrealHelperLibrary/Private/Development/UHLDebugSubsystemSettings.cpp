// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/UHLDebugSubsystemSettings.h"

#include "Core/UHLGameplayTags.h"
#include "Subsystems/DebugSubsystem/UHLDebugCategory.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Abilities.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Attributes.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Effects.h"
#include "Subsystems/DebugSubsystem/DebugCategories/DCC_InputSystem_EnhancedInput.h"


void UUHLDebugSubsystemSettings::AddOrUpdateDefualtUHLDebugCategories()
{
    TArray<FUHLDebugCategory> DEFAULT_UHL_DEBUG_CATEGORIES = {};

    FUHLDebugCategory AbilitySystemAttributesDebugCategory = {};
    AbilitySystemAttributesDebugCategory.Name = "AbilitySystem Attributes";
    AbilitySystemAttributesDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem_Attributes };
    AbilitySystemAttributesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    AbilitySystemAttributesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem);
    AbilitySystemAttributesDebugCategory.Components = { UDCC_AbilitySystem_Attributes::StaticClass() };
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilitySystemAttributesDebugCategory);

    FUHLDebugCategory AbilitySystemEffectsDebugCategory = {};
    AbilitySystemEffectsDebugCategory.Name = "AbilitySystem Effects";
    AbilitySystemEffectsDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem_Effects };
    AbilitySystemEffectsDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    AbilitySystemEffectsDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem);
    AbilitySystemEffectsDebugCategory.Components = { UDCC_AbilitySystem_Effects::StaticClass() };
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilitySystemEffectsDebugCategory);

    FUHLDebugCategory AbilitySystemAbilitiesDebugCategory = {};
    AbilitySystemAbilitiesDebugCategory.Name = "AbilitySystem Abilities";
    AbilitySystemAbilitiesDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem_Abilities };
    AbilitySystemAbilitiesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    AbilitySystemAbilitiesDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem);
    AbilitySystemAbilitiesDebugCategory.Components = { UDCC_AbilitySystem_Abilities::StaticClass() };
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilitySystemAbilitiesDebugCategory);

    FUHLDebugCategory EnhancedInputSystemDebugCategory = {};
    EnhancedInputSystemDebugCategory.Name = "EnhancedInputSystem";
    EnhancedInputSystemDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem };
    EnhancedInputSystemDebugCategory.Blocks.AddTag(UHLGameplayTags::TAG_UHL_DebugCategory_AbilitySystem);
    EnhancedInputSystemDebugCategory.Components = { UDCC_InputSystem_EnhancedInput::StaticClass() };
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(EnhancedInputSystemDebugCategory);

    FUHLDebugCategory AbilityInputCacheDebugCategory = {};
    AbilityInputCacheDebugCategory.Name = "AbilityInputCache";
    AbilityInputCacheDebugCategory.Tags = FGameplayTagContainer{ UHLGameplayTags::TAG_UHL_AbilityInputCache_Catching };
    DEFAULT_UHL_DEBUG_CATEGORIES.Add(AbilityInputCacheDebugCategory);

    for (const FUHLDebugCategory& DebugCategory : DEFAULT_UHL_DEBUG_CATEGORIES)
    {
        DebugCategories.Add(DebugCategory);
    }
}

FName UUHLDebugSubsystemSettings::GetCategoryName() const
{
    return FApp::GetProjectName();
}

#if WITH_EDITOR
void UUHLDebugSubsystemSettings::PostInitProperties()
{
    Super::PostInitProperties();
    if (DebugCategories.IsEmpty())
    {
        AddOrUpdateDefualtUHLDebugCategories();
    }

    RecreateEnabledDebugCategoriesList();
}

void UUHLDebugSubsystemSettings::PreEditChange(FProperty* PropertyAboutToChange)
{
    if (PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategories)
        || PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategoriesNames))
    {
        LastEnabledDebugCategories = TMap(EnabledDebugCategories);
        LastEnabledDebugCategoriesNames = TMap(EnabledDebugCategoriesNames);
    }

    Super::PreEditChange(PropertyAboutToChange);
}

void UUHLDebugSubsystemSettings::PreEditChange(class FEditPropertyChain& PropertyAboutToChange)
{
    // FName PropertyName = PropertyAboutToChange.GetActiveNode()->GetValue()->GetName();
    //
    // bool bNotEditingEnabledCategoriesKey = PropertyName != "EnabledDebugCategories_Key";
    // if (bNotEditingEnabledCategoriesKey)
    // {
    //     Super::PostEditChangeChainProperty(PropertyChangedEvent);
    // }
    Super::PreEditChange(PropertyAboutToChange);
}

bool UUHLDebugSubsystemSettings::CanEditChange(const FProperty* InProperty) const
{
    // FName PropertyName = InProperty->Get;
    // bool bNotEditingEnabledCategoriesKey = PropertyName != "EnabledDebugCategories_Key";
    return Super::CanEditChange(InProperty);
}

bool UUHLDebugSubsystemSettings::CanEditChange(const FEditPropertyChain& PropertyChain) const
{
    return Super::CanEditChange(PropertyChain);
}

void UUHLDebugSubsystemSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UUHLDebugSubsystemSettings::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
    FName PropertyName = PropertyChangedEvent.GetPropertyName();

    bool bEditingEnabledCategoriesKey = PropertyName == "EnabledDebugCategories_Key";
    if (bEditingEnabledCategoriesKey)
    {
        RecreateEnabledDebugCategoriesList();
        return;
    }

    Super::PostEditChangeChainProperty(PropertyChangedEvent);

    bool bEditingEnabledDebugCategories = PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategories)
        && LastEnabledDebugCategories.Num() == EnabledDebugCategories.Num();
    bool bEditingEnabledDebugCategoriesNames = PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategoriesNames)
        && LastEnabledDebugCategoriesNames.Num() == EnabledDebugCategoriesNames.Num();


    // disable other DebugCategories by "Blocks" tags
    if (bEditingEnabledDebugCategories || bEditingEnabledDebugCategoriesNames)
    {
        FGameplayTag ChangedDebugCategoryTag = FGameplayTag::EmptyTag;
        if (!bDisplayShortNames)
        {
			for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : EnabledDebugCategories)
			{
				if (LastEnabledDebugCategories[EnabledDebugCategory.Key] != EnabledDebugCategory.Value
					&& EnabledDebugCategory.Value == true)
				{
					ChangedDebugCategoryTag = EnabledDebugCategory.Key;
				}
			}
        }
        else
        {
            for (const TTuple<FString, bool>& EnabledDebugCategory : EnabledDebugCategoriesNames)
            {
                if (LastEnabledDebugCategoriesNames[EnabledDebugCategory.Key] != EnabledDebugCategory.Value
                    && EnabledDebugCategory.Value == true)
                {
                    FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
					{
						return DebugCategory.Name == EnabledDebugCategory.Key;
					});
                    ChangedDebugCategoryTag = UHLDebugCategory->Tags.First();
                }
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

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, DebugCategories)
        || PropertyName == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, UHLDefaultDebugCategories)
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

    EnabledDebugCategoriesNames.Empty();
    for (TTuple<FGameplayTag, bool> EnabledDebugCategory : EnabledDebugCategories)
    {
        FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
        {
            return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
        });
        if (UHLDebugCategory != nullptr)
        {
            EnabledDebugCategoriesNames.Add(UHLDebugCategory->Name, EnabledDebugCategory.Value);
        }
    }
}

void UUHLDebugSubsystemSettings::LoadGameplayTagTables(bool bAllowAsyncLoad) const
{
#if !WITH_EDITOR
    // If we're a cooked build and in a safe spot, start an async load so we can pipeline it
    if (bAllowAsyncLoad && !IsLoading() && GameplayTagTableList.Num() > 0)
    {
        for (FSoftObjectPath DataTablePath : GameplayTagTableList)
        {
            LoadPackageAsync(DataTablePath.GetLongPackageName());
        }

        return;
    }
#endif // !WITH_EDITOR

    for (FSoftObjectPath DataTablePath : DebugCategoriesGameplayTagsTableList)
    {
        UDataTable* TagTable = LoadObject<UDataTable>(nullptr, *DataTablePath.ToString(), nullptr, LOAD_None, nullptr);

        // Handle case where the module is dynamically-loaded within a LoadPackage stack, which would otherwise
        // result in the tag table not having its RowStruct serialized in time. Without the RowStruct, the tags manager
        // will not be initialized correctly.
        if (TagTable)
        {
            FLinkerLoad* TagLinker = TagTable->GetLinker();
            if (TagLinker)
            {
                TagTable->GetLinker()->Preload(TagTable);
            }
        }
    }
}
