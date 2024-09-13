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
    //~UDeveloperSettings interface
    virtual FName GetCategoryName() const override;
    //~End of UDeveloperSettings interface

    virtual void PostInitProperties() override;

    UPROPERTY(config, EditAnywhere, Category="EnabledDebugCategories", meta=(FullyExpand=true, ForceInlineRow))
    TMap<FGameplayTag, bool> EnabledDebugCategories = {};


    UPROPERTY(config, EditAnywhere, Category="DebugCategories", meta=(FullyExpand=true))
    bool bEnabledDebugCategoriesOnStart = true;
    UPROPERTY(config, EditAnywhere, Category="DebugCategories", meta=(FullyExpand=true))
    TArray<FUHLDebugCategory> DebugCategories = {};
};
