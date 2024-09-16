// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UHLDebugCategory.generated.h"

class UUHLDebugCategoryComponent;

USTRUCT(BlueprintType)
struct FUHLDebugCategory
{
    GENERATED_BODY()

    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bActive = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ShortName = "";
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTagContainer Tags;

    // deactivates all debug categories with "Blocks" tags and activate current
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTagContainer Blocks;   // blocks other debug categories activation with specified tags
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // FGameplayTagContainer Deactivates; // deactivates other debug categories
    /** defines what to do like GameplayEffects components e.g.
    I want component that enables AbilitySystem debug, I write component and add it here,
    so when DebugCategory activated that simple component Activates, when DebugCategory
    deactivated component Deactivates, such simple **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceShowPluginContent))
    TArray<TSubclassOf<UUHLDebugCategoryComponent>> Components;
    // for UI, background color and so on
    UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
    FLinearColor Color = FLinearColor::MakeRandomColor();
    // for some specific categories like "Collisions" that keeps activated between sessions
    // TODO probably all activated debug categories should be deactivated on end?
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bForceComponentsDeactivateOnEnd = false;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(InlineEditConditionToggle))
    // bool bPriority = false;
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bPriority"))
    // int32 Priority = 0;

    bool bEnabled = false;

    bool TryActivate(UObject* ContextObj);
    void TryDeactivate(UObject* ContextObj);

    bool operator==(const FUHLDebugCategory& Other) const
    {
        return ShortName == Other.ShortName && Tags == Other.Tags;
    }

private:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPROPERTY()
    TArray<UUHLDebugCategoryComponent*> InstancedComponents = {};

    UUHLDebugCategoryComponent* GetOrCreateDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj);
};
