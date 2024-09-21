// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"
#include "UHLDebugCategory.generated.h"


class UUHLDebugCategoryComponent;

USTRUCT(BlueprintType)
struct FUHLDebugCategory
{
    GENERATED_BODY()

    // TODO:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // bool bActive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name = "";
    // Tags associated with this debug category, like GameplayAbilities category can be activated/deactivated by tag
    // WARNING for better experience tags are filtered add child to "UHL.DebugCategory" or "DebugCategory"
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories = DEBUG_CATEGORIES_TAGS_FILTER))
    FGameplayTagContainer Tags = {};

    // What DebugCategories this DebugCategory blocks. On enabling this DebugCategory it will disable other debug categories that match "Blocks" tags
    // WARNING for better experience tags are filtered add child to "UHL.DebugCategory" or "DebugCategory"
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories = DEBUG_CATEGORIES_TAGS_FILTER))
    FGameplayTagContainer Blocks = {};   // blocks other debug categories activation with specified tags
    // TODO BlockedBy - what categories blocks it
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories = "UHL.DebugCategory"))
    FGameplayTagContainer BlockedBy = {};

    /** defines what to do when DebugCategory enabling, works a bit similar to GameplayAbilities and GameplayEffects components.
    Example - I want component that enables AbilitySystem debug, I write DebugCategoryComponent(C++/BP) and add it here,
    so when DebugCategory enables that simple component "activates", when DebugCategory
    disabled component "deactivates", such simple **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceShowPluginContent))
    TArray<TSubclassOf<UUHLDebugCategoryComponent>> Components;
    // for UI, background color and so on
    UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
    FLinearColor Color = FLinearColor::MakeRandomColor();
    // if DebugCategory requires PlayerController it can't be enabled before "SetUpCategoriesThatRequiresPlayerController" being called
    // Mostly you want to add "SetUpCategoriesThatRequiresPlayerController" in your "PlayerController.BeginPlay"
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresPlayerControllerToEnable = true;
    UPROPERTY()
    bool bIsDefaultUHLDebugCategory = false;

    bool TryEnable(UObject* ContextObj);
    void TryDisable(UObject* ContextObj);
    bool GetIsEnabled() const { return bIsEnabled; };

    bool operator==(const FUHLDebugCategory& Other) const
    {
        return Name == Other.Name;
        // not sure checking tags that might change is good idea
        // return ShortName == Other.ShortName && Tags == Other.Tags;
    }

private:
    bool bIsEnabled = false;

    UPROPERTY()
    TArray<UUHLDebugCategoryComponent*> InstancedComponents = {};

    UUHLDebugCategoryComponent* GetOrCreateDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj);
    UUHLDebugCategoryComponent** GetDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj);
};
