// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UHLDebugSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUHLDebugCategoryEnabled, FGameplayTag, DebugCategoryTag, bool, bEnabled);

// TODO force activation with "MMB" click
UCLASS(Blueprintable)
class UUHLDebugCategoryComponent : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    bool Activate(UObject* ContextObject, APlayerController* PlayerController);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    void Deactivate(UObject* ContextObject, APlayerController* PlayerController);
    // UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    // void CanActivate();
};

USTRUCT(BlueprintType)
struct FUHLDebugCategory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bActive = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag Tag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor Color = FLinearColor::MakeRandomColor();
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTagContainer Blocks;   // blocks other debug categories activation with specified tags
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTagContainer Deactivates; // deactivates other debug categories
    /** defines what to do like GameplayEffects components e.g.
    I want component that enables AbilitySystem debug, I write component and add it here,
    so when DebugCategory activated that simple component Activates, when DebugCategory
    deactivated component Deactivates, such simple **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<UUHLDebugCategoryComponent>> Components;

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
