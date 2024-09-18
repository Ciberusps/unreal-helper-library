// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"
#include "UHLDebugSubsystemSettings.generated.h"

class UEnvQuery;
class AEncounterPositionPoints;

/**
 *
 */
UCLASS(config=EditorPerProjectUserSettings, MinimalAPI, PrioritizeCategories="DebugCategories")
class UUHLDebugSubsystemSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
    // TODO
    // UPROPERTY(config, EditAnywhere, Category="DebugCategories")
    bool bDisplayShortNames = false;
    UPROPERTY(config, EditAnywhere, Category="DebugCategories", meta=(FullyExpand=true, ForceInlineRow, EditCondition="!bDisplayShortNames", EditConditionHides, NoClear /**, ReadOnlyKeys **/))
    TMap<FGameplayTag, bool> EnabledDebugCategories = {};

    // only for visual, changes values in EnabledDebugCategories
    // and then gets updates self state with EnabledDebugCategories values
    // это чисто визуалыч меняющий значения в EnabledDebugCategories,
    // а сам только отображает состояние
    // UPROPERTY(config, EditAnywhere, Category="DebugCategories", meta=(FullyExpand=true, ForceInlineRow, EditCondition="bDisplayShortNames", EditConditionHides))
    TMap<FString, bool> EnabledDebugCategoriesNames = {};

    // UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition", meta=(FullyExpand=true))
    bool bEnableDebugCategoriesOnStart = true;
    // TODO validate short names uniqueness or just append with random symbols if not unique (NoElementDuplicate - dont work)
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition", meta=(TitleProperty="ShortName", NoElementDuplicate))
    TArray<FUHLDebugCategory> DebugCategories = {};
    // Final decision - array > map, not often edited but easier to change priority
    // UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    TMap<FString, FUHLDebugCategory> DebugCategoriesMap = {};

// TODO: add default UHL categories that can be disabled by click UPD better to add EditorCallable function
// cause ordering is important
    // useful for future updates, so user don't need to reset/merge their debug categories
    // if dont want to use and see UHL DebugCategories in EnabledDebugCategories at all
    // UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    bool bExcludeUHLDebugCategories = false;
    // UPROPERTY(config, EditAnywhere, Category="DebugCategoriesDefinition")
    TArray<FUHLDebugCategory> UHLDefaultDebugCategories = {};

    // TODO: choosing debug subsystem class??? user can extend debug subsystem with own things?
    TSubclassOf<UUHLDebugSubsystem> UHLDebugSubsystemClass;

    // Called if no debug categories found
    UFUNCTION(BlueprintCallable, CallInEditor)
    void AddOrUpdateDefualtUHLDebugCategories();

protected:
    //~UDeveloperSettings interface
    virtual FName GetCategoryName() const override;
    //~End of UDeveloperSettings interface

#if WITH_EDITOR
    virtual void PostInitProperties() override;
    virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
    virtual void PreEditChange(class FEditPropertyChain& PropertyAboutToChange) override;
    virtual bool CanEditChange(const FProperty* InProperty) const override;
    virtual bool CanEditChange(const FEditPropertyChain& PropertyChain) const override;
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

private:
    TMap<FGameplayTag, bool> LastEnabledDebugCategories;
    TMap<FString, bool> LastEnabledDebugCategoriesNames;

    void RecreateEnabledDebugCategoriesList();
    void UpdateEnabledDebugCategoriesList();
};
