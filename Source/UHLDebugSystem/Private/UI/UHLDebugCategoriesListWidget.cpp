// Pavel Penkov 2025 All Rights Reserved.


#include "UI/UHLDebugCategoriesListWidget.h"

#include "Engine/GameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ButtonSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Development/UHLDebugSystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DebugCategoryButtonWidget.h"
#include "UHLDebugSystemSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategoriesListWidget)

bool UUHLDebugCategoriesListWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    if(!HasAnyFlags(RF_ClassDefaultObject))
    {
        // root have to be initialized in Initialize function, otherwise it will not work, donno exactly why.
        ScrollBox = WidgetTree->ConstructWidget<UScrollBox>();
        ScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Always);
        ScrollBox->SetVisibility(ESlateVisibility::Visible);
        WidgetTree->RootWidget = ScrollBox;
    }

    return bIsWidgetInitialized;
}

void UUHLDebugCategoriesListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ScrollBox->ClearChildren();

    UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSystemSubsystem>();

    const TArray<FUHLDebugCategory>& UHLDebugCategories = UHLDebugSubsystem->GetDebugCategories();
    for (const FUHLDebugCategory& UHLDebugCategory : UHLDebugCategories)
    {
        UDebugCategoryButtonWidget* UHLDebugCategoryButton = WidgetTree->ConstructWidget<UDebugCategoryButtonWidget>();
        UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(UHLDebugCategoryButton));
        UHLDebugCategoryButton->SetUp(UHLDebugCategory);
        UHLDebugCategoryButton->OnMadeClick.AddUniqueDynamic(this, &UUHLDebugCategoriesListWidget::OnButtonClicked);

        ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 10));
        ScrollBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
    }
}

void UUHLDebugCategoriesListWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    const UUHLDebugSystemSettings* DeveloperSettings = GetDefault<UUHLDebugSystemSettings>();
    for (TTuple<FGameplayTag, bool> EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
    {
        UDebugCategoryButtonWidget* UHLDebugCategoryButton = WidgetTree->ConstructWidget<UDebugCategoryButtonWidget>();
        UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(UHLDebugCategoryButton));

        const FUHLDebugCategory* UHLDebugCategory = DeveloperSettings->DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
		{
			return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
		});
        if (UHLDebugCategory != nullptr)
        {
			UHLDebugCategoryButton->SetUp(*UHLDebugCategory);
			ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 10));
        }
    }
}

void UUHLDebugCategoriesListWidget::OnButtonClicked(UDebugCategoryButtonWidget* Button, FGameplayTag DebugCategoryGameplayTag)
{
    if (!Button) return;

    UHLDebugSubsystem->ToggleDebugCategory(DebugCategoryGameplayTag);
    Button->UpdateCheckboxState(UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryGameplayTag));
}
