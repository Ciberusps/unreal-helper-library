// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"
#include "UHLDebugSubsystemSettings.generated.h"


/**
 *
 */
UCLASS(config="EditorPerProjectUserSettings", MinimalAPI, PrioritizeCategories="DebugCategories")
class UUHLDebugSubsystemSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
    UPROPERTY(config, EditAnywhere, Category="DebugCategories", meta=(FullyExpand=true, ForceInlineRow, EditCondition="!bDisplayShortNames", EditConditionHides, NoClear /**, ReadOnlyKeys **/))
    TMap<FGameplayTag, bool> EnabledDebugCategories = {};

    // UPROPERTY(config, EditAnywhere, Category="DebugCategoriesSettings", meta=(FullyExpand=true))
    bool bEnableDebugCategoriesOnStart = true;

    // TODO: add default UHL categories that can be disabled by click UPD better to add EditorCallable function
    // cause ordering is important
    // useful for future updates, so user don't need to reset/merge their debug categories

    // if dont want to use and see UHL DebugCategories in EnabledDebugCategories at all
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesSettings")
    bool bExcludeDefaultUHLDebugCategories = false;

    // TODO validate short names uniqueness or just append with random symbols if not unique (NoElementDuplicate - dont work)
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesSettings", meta=(TitleProperty="Name", NoElementDuplicate))
    TArray<FUHLDebugCategory> DebugCategories = {};

    // TODO: choosing debug subsystem class??? user can extend debug subsystem with own things?
    // TSubclassOf<UUHLDebugSubsystem> UHLDebugSubsystemClass;

    static TArray<FUHLDebugCategory> GET_DEFAULT_UHL_DEBUG_CATEGORIES();

protected:
//~UDeveloperSettings interface
    virtual FName GetCategoryName() const override;
//~End of UDeveloperSettings interface

#if WITH_EDITOR
    virtual void PostInitProperties() override;
    virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
    virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

private:
    TMap<FGameplayTag, bool> LastEnabledDebugCategories;

    void RecreateEnabledDebugCategoriesList();
    void UpdateEnabledDebugCategoriesList();
    void UpdateDefaultUHLDebugCategories();
};
