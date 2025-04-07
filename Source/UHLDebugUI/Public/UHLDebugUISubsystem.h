// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UHLDebugCategory.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UHLDebugUISubsystem.generated.h"


UCLASS()
class UHLDEBUGUI_API UUHLDebugUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    UUHLDebugUISubsystem();

    // By default called on Subsystem Initialize, no need to call by self
    // should be called as soon as possible - better on GameInstance.Init
    UFUNCTION(BlueprintCallable, Category="UHLDebugSubsystem")
    void SetUp();
    // should be called when player controller available, e.g. in PlayerController.BeginPlay
    UFUNCTION(BlueprintCallable, Category="UHLDebugSubsystem")
    void SetUpCategoriesThatRequiresPlayerController();

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    bool bSetupped = false;
    bool bSetUpCategoriesThatRequiresPlayerController = false;
    bool bIsSetuping = true;

    UPROPERTY()
    TArray<FUHLDebugCategory> DebugCategories = {};
    UPROPERTY()
    TSoftClassPtr<UUHLDebugWidget> UHLDebugWidgetClass;
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
