// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ScrollBox.h"
#include "UHLDebugCategoriesListWidget.generated.h"

class UUHLDebugSubsystem;
class UDebugCategoryButtonWidget;
class UCanvasPanel;
/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UUHLDebugCategoriesListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

private:
    UPROPERTY()
    UScrollBox* ScrollBox;
    TWeakObjectPtr<UUHLDebugSubsystem> UHLDebugSubsystem;
    UFUNCTION()
    void OnButtonClicked(UDebugCategoryButtonWidget* Button, FGameplayTag DebugCategoryGameplayTag);
};
