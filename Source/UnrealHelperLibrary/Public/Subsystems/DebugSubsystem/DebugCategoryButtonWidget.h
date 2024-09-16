// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UHLDebugCategory.h"
#include "Blueprint/UserWidget.h"
#include "DebugCategoryButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDebugCategoryButtonClick, UDebugCategoryButtonWidget*, Button, FGameplayTag, DebugCategoryTag);

class UUHLDebugSubsystem;
struct FUHLDebugCategory;
class UTextBlock;
class UCheckBox;
class UHorizontalBox;
class UButton;
/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UDebugCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetUp(const FUHLDebugCategory& UHLDebugCategory_In);
    void UpdateCheckboxState(bool bEnabled_In);

    FDebugCategoryButtonClick OnMadeClick;

protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;
    // TODO use EnabledDebugCategories
    // virtual void NativePreConstruct() override;
private:
	UPROPERTY()
    UButton* Button;
    UPROPERTY()
    UHorizontalBox* HorizontalBox;
    UPROPERTY()
    UCheckBox* CheckBox;
    UPROPERTY()
    UTextBlock* TextBlock;
    UPROPERTY()
    FUHLDebugCategory UHLDebugCategory;
    TWeakObjectPtr<UUHLDebugSubsystem> UHLDebugSubsystem;

    UFUNCTION()
    void OnButtonClicked();
};
