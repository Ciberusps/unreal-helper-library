// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UHLDebugSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUHLDebugCategoryEnabled, FGameplayTag, DebugCategoryTag, bool, bEnabled);

// TODO force activation with "MMB" click
UCLASS(Abstract, Blueprintable)
class UNREALHELPERLIBRARY_API UUHLDebugCategoryComponent : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    void Activate(UObject* ContextObject);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    void Deactivate(UObject* ContextObject);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    bool CanActivate(UObject* ContextObject) const;
};

USTRUCT(BlueprintType)
struct FUHLDebugCategory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
    FLinearColor Color = FLinearColor::MakeRandomColor();

    bool TryActivate(UObject* ContextObj, APlayerController* PlayerController);

private:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnabled = false;
    UPROPERTY()
    TArray<UUHLDebugCategoryComponent*> InstancedComponents = {};
};

UCLASS()
class UNREALHELPERLIBRARY_API UUHLDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    UUHLDebugSubsystem();

    UPROPERTY(BlueprintAssignable)
    FOnUHLDebugCategoryEnabled OnDebugCategoryEnabled;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable)
    void SetUp();
    UFUNCTION(Exec, BlueprintCallable)
    bool IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const;
    UFUNCTION(Exec, BlueprintCallable)
    void EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bIsEnabled);

private:
    TArray<FUHLDebugCategory> DebugCategories = {};
    // TMap<FGameplayTag, FUHLDebugCategory> EnabledDebugCategories;
    // TArray<FGameplayTag, FUHLDebugCategory> EnabledDebugCategoriesComponent = {};

    APlayerController* GetPlayerController() const;

    // UFUNCTION()
    // void OnDebugCategoryEnabledInternal(EDebugCategory DebugCategory, bool bEnabled);
};
