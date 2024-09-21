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

    FOnUHLDebugCategoryChanged OnDebugCategoryChanged;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // By default called on Subsystem Initialize, no need to call by self
    // should be called as soon as possible - better on GameInstance.Init
    UFUNCTION(BlueprintCallable, Category="UHLDebugSubsystem")
    void SetUp();
    // should be called when player controller available, e.g. in PlayerController.BeginPlay
    UFUNCTION(BlueprintCallable, Category="UHLDebugSubsystem")
    void SetUpCategoriesThatRequiresPlayerController();

    // TODO filter gameplayTags, allow using only values from DebugCategories `meta=(Categories="")`
    // probably "Godreaper.DebugCategories." required, or "ProjectName.DebugCategories"...
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    bool IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const;
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    void EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable);
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem")
    void ToggleDebugCategory(const FGameplayTag DebugCategoryTag);

    const TArray<FUHLDebugCategory>& GetDebugCategories() const { return DebugCategories; }

private:
    bool bSetupped = false;
    bool bSetUpCategoriesThatRequiresPlayerController = false;
    bool bIsSetuping = true;

    UPROPERTY()
    TArray<FUHLDebugCategory> DebugCategories = {};
    UPROPERTY()
    TSubclassOf<UUHLDebugWidget> UHLDebugWidgetClass;
    UPROPERTY()
    UUHLDebugWidget* DebugWidgetInstance = nullptr;

    APlayerController* GetPlayerController() const;
    UUHLAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;
    UUHLDebugWidget* GetOrCreateUHLDebugWidget();
    UFUNCTION()
    void ToggleAbilityInputDebug();
    UFUNCTION()
    void OnDebugCategoryChangedInternal(FGameplayTag DebugCategoryTag, bool bEnabled);
};
