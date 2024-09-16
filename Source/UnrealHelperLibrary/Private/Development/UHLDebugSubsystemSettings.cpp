// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/UHLDebugSubsystemSettings.h"

#include "Subsystems/DebugSubsystem/UHLDebugCategory.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

FName UUHLDebugSubsystemSettings::GetCategoryName() const
{
    return FApp::GetProjectName();
}

void UUHLDebugSubsystemSettings::PostInitProperties()
{
    Super::PostInitProperties();
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
						return DebugCategory.ShortName == EnabledDebugCategory.Key;
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
            EnabledDebugCategoriesNames.Add(UHLDebugCategory->ShortName, EnabledDebugCategory.Value);
        }
    }
}
