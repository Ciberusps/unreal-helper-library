// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "UHLDebugSubsystemSettings.generated.h"

struct FUHLDebugCategory;
class UEnvQuery;
class AEncounterPositionPoints;

/**
 *
 */
UCLASS(config=EditorPerProjectUserSettings, MinimalAPI)
class UUHLDebugSubsystemSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
    // TODO
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    bool bDisplayShortNames = false;
    UPROPERTY(config, EditAnywhere, Category="DebugCategories", meta=(FullyExpand=true, ForceInlineRow, EditCondition="!bDisplayShortNames", EditConditionHides))
    TMap<FGameplayTag, bool> EnabledDebugCategories = {};

    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition", meta=(FullyExpand=true))
    bool bEnabledDebugCategoriesOnStart = true;
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    TArray<FUHLDebugCategory> DebugCategories = {};

    // if dont want to use and see UHL DebugCategories in EnabledDebugCategories at all
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    bool bExcludeUHLDebugCategories = false;
    // TODO
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    TArray<FUHLDebugCategory> UHLDefaultDebugCategories = {};

protected:
    //~UDeveloperSettings interface
    virtual FName GetCategoryName() const override;
    //~End of UDeveloperSettings interface

    virtual void PostInitProperties() override;
    virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:
    TMap<FGameplayTag, bool> LastEnabledDebugCategories;

    void UpdateEnabledDebugCategoriesList();
};
