// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Misc/App.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "UHLDebugSubsystem.h"
#include "UHLDebugSubsystemSettings.generated.h"

/**
 *
 */
UCLASS(config="Game", defaultconfig, PrioritizeCategories="DebugCategories")
class UHLDEBUG_API UUHLDebugSubsystemSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="DebugCategories", NoClear, meta=(FullyExpand=true, ForceInlineRow /**, ReadOnlyKeys **/))
    TMap<FGameplayTag, bool> EnabledDebugCategories = {};

    // don't use/show default UHLDebugCategories
    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesSettings")
    bool bExcludeDefaultUHLDebugCategories = false;

    UPROPERTY(config, EditAnywhere, Category="DebugCategoriesSettings", meta=(TitleProperty="Name", NoElementDuplicate))
    TArray<FUHLDebugCategory> DebugCategories = {};

    // TODO: choosing debug subsystem class??? user can extend debug subsystem with own things?
    // TSubclassOf<UUHLDebugSubsystem> UHLDebugSubsystemClass;

    static TArray<FUHLDebugCategory> GET_DEFAULT_UHL_DEBUG_CATEGORIES();

protected:
//~UDeveloperSettings interface
    virtual FName GetCategoryName() const override { return FApp::GetProjectName(); };
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
