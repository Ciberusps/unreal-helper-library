// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UHLDebugCategory.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UHLDebugSubsystem.generated.h"


class UUHLAbilitySystemComponent;
class UUHLDebugWidget;
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

    UFUNCTION(BlueprintCallable, Category="UHLDebugSubsystem")
    void SetUp();
    // TODO filter gameplayTags, allow using only values from DebugCategories `meta=(Categories="")`
    // probably "Godreaper.DebugCategories." required, or "ProjectName.DebugCategories"...
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    bool IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const;
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    void EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable);
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    void ToggleDebugCategory(const FGameplayTag DebugCategoryTag);

    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    void ToggleAbilityInputDebug();

    const TArray<FUHLDebugCategory>& GetDebugCategories() const { return DebugCategories; }

private:
    bool bSetupped = false;
    bool bIsSetuping = true;
    TArray<FUHLDebugCategory> DebugCategories = {};
    // TMap<FGameplayTag, FUHLDebugCategory> EnabledDebugCategories;
    // TArray<FGameplayTag, FUHLDebugCategory> EnabledDebugCategoriesComponent = {};

    UPROPERTY()
    TSubclassOf<UUHLDebugWidget> UHLDebugWidgetClass;
    UPROPERTY()
    UUHLDebugWidget* DebugWidgetInstance = nullptr;

    APlayerController* GetPlayerController() const;
    UUHLAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;
    UUHLDebugWidget* GetOrCreateUHLDebugWidget();
    UFUNCTION()
    void OnDebugCategoryChangedInternal(FGameplayTag DebugCategoryTag, bool bEnabled);
};
