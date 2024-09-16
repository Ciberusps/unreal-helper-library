// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/UI/UHLDebugCategoriesListWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/DebugCategoryButtonWidget.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

bool UUHLDebugCategoriesListWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    if(!HasAnyFlags(RF_ClassDefaultObject) )
    {
        // root have to be initialized in Initialize function, otherwise it will not work, donno exactly why.
        // CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>();
        ScrollBox = WidgetTree->ConstructWidget<UScrollBox>();
        WidgetTree->RootWidget = ScrollBox;
    }

    return bIsWidgetInitialized;
}

void UUHLDebugCategoriesListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSubsystem>();

    const TArray<FUHLDebugCategory>& UHLDebugCategories = UHLDebugSubsystem->GetDebugCategories();
    for (const FUHLDebugCategory& UHLDebugCategory : UHLDebugCategories)
    {
        UDebugCategoryButtonWidget* UHLDebugCategoryButton = WidgetTree->ConstructWidget<UDebugCategoryButtonWidget>(UDebugCategoryButtonWidget::StaticClass(), FName(UHLDebugCategory.ShortName));
        UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(UHLDebugCategoryButton));
		UHLDebugCategoryButton->SetUp(UHLDebugCategory);
        UHLDebugCategoryButton->OnMadeClick.AddUniqueDynamic(this, &UUHLDebugCategoriesListWidget::OnMadeClick);

		ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 10));
    }
}

void UUHLDebugCategoriesListWidget::OnMadeClick(UDebugCategoryButtonWidget* Button, FGameplayTag DebugCategoryGameplayTag)
{
    UHLDebugSubsystem->ToggleDebugCategory(DebugCategoryGameplayTag);
    Button->UpdateCheckboxState(UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryGameplayTag));
}
