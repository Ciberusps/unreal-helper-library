// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ScrollBox.h"
#include "UHLDebugCategoriesListWidget.generated.h"

class UUHLDebugSystemSubsystem;
class UDebugCategoryButtonWidget;
class UCanvasPanel;
/**
 *
 */
UCLASS()
class UHLDEBUGSYSTEM_API UUHLDebugCategoriesListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

private:
    UPROPERTY()
    UScrollBox* ScrollBox;
    TWeakObjectPtr<UUHLDebugSystemSubsystem> UHLDebugSubsystem;
    UFUNCTION()
    void OnButtonClicked(UDebugCategoryButtonWidget* Button, FGameplayTag DebugCategoryGameplayTag);
};
