// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UHLDebugCategory.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UHLDebugSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUHLDebugCategoryChanged, FGameplayTag, DebugCategoryTag, bool, bEnabled);

UCLASS()
class UNREALHELPERLIBRARY_API UUHLDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    UUHLDebugSubsystem();

    // UPROPERTY(BlueprintAssignable)
    FOnUHLDebugCategoryChanged OnDebugCategoryChanged;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable)
    void SetUp();
    // TODO filter gameplayTags, allow using only values from DebugCategories `meta=(Categories="")`
    // probably "Godreaper.DebugCategories." required, or "ProjectName.DebugCategories"...
    UFUNCTION(Exec, BlueprintCallable)
    bool IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const;
    UFUNCTION(Exec, BlueprintCallable)
    void EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable);

private:
    bool bSetupped = false;
    TArray<FUHLDebugCategory> DebugCategories = {};
    // TMap<FGameplayTag, FUHLDebugCategory> EnabledDebugCategories;
    // TArray<FGameplayTag, FUHLDebugCategory> EnabledDebugCategoriesComponent = {};

    APlayerController* GetPlayerController() const;

    // UFUNCTION()
    // void OnDebugCategoryEnabledInternal(EDebugCategory DebugCategory, bool bEnabled);
};
