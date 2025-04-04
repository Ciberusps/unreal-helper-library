// Pavel Penkov 2025 All Rights Reserved.

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
class UHLDEBUGSYSTEM_API UDebugCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetUp(const FUHLDebugCategory& UHLDebugCategory_In);
    void UpdateCheckboxState(bool bEnabled_In);

    FDebugCategoryButtonClick OnMadeClick;

protected:
    virtual bool Initialize() override;
    virtual void NativePreConstruct() override;

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

    UFUNCTION()
    void OnButtonClicked();
    UFUNCTION()
    void OnDebugCategoryChanged(FGameplayTag DebugCategoryTag_In, bool bEnabled_In);
};
