// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/UHLDebugSubsystemSettings.h"

#include "Subsystems/UHLDebugSubsystem.h"

FName UUHLDebugSubsystemSettings::GetCategoryName() const
{
    return FApp::GetProjectName();
}

void UUHLDebugSubsystemSettings::PostInitProperties()
{
    Super::PostInitProperties();
    UpdateEnabledDebugCategoriesList();
}

void UUHLDebugSubsystemSettings::PreEditChange(FProperty* PropertyAboutToChange)
{
    if (PropertyAboutToChange->GetName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategories))
    {
        LastEnabledDebugCategories = TMap(EnabledDebugCategories);
    }

    Super::PreEditChange(PropertyAboutToChange);
}

void UUHLDebugSubsystemSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UUHLDebugSubsystemSettings, EnabledDebugCategories)
        && LastEnabledDebugCategories.Num() == EnabledDebugCategories.Num())
    {
        FGameplayTag ChangedDebugCategoryTag = FGameplayTag::EmptyTag;
        for (TTuple<FGameplayTag, bool> EnabledDebugCategory : EnabledDebugCategories)
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
                return DebugCategory.Tags.HasAny(FGameplayTagContainer(ChangedDebugCategoryTag));
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
            }
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
