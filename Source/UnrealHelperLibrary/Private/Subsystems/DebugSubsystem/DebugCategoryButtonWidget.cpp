// Copyright (c) 2024 Pavel Penkov


#include "Subsystems/DebugSubsystem/DebugCategoryButtonWidget.h"

#include "Engine/GameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CheckBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Development/UHLDebugSubsystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/UHLDebugCategory.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DebugCategoryButtonWidget)

void UDebugCategoryButtonWidget::SetUp(const FUHLDebugCategory& UHLDebugCategory_In)
{
    UHLDebugCategory = UHLDebugCategory_In;
    TextBlock->SetText(FText::FromString(UHLDebugCategory_In.Name));
    UpdateCheckboxState(UHLDebugCategory_In.GetIsEnabled());
    Button->SetBackgroundColor(UHLDebugCategory.Color);
    Button->OnClicked.AddUniqueDynamic(this, &UDebugCategoryButtonWidget::OnButtonClicked);
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    if (GameInstance)
    {
        UUHLDebugSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSubsystem>();
        UHLDebugSubsystem->OnDebugCategoryChanged.AddUniqueDynamic(this, &UDebugCategoryButtonWidget::OnDebugCategoryChanged);
    }
}

void UDebugCategoryButtonWidget::UpdateCheckboxState(bool bEnabled_In)
{
    CheckBox->SetCheckedState(bEnabled_In ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

bool UDebugCategoryButtonWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    // more correct way to avoid error. than in original post.
    if(!HasAnyFlags(RF_ClassDefaultObject))
    {
        // root have to be initialized in Initialize function, otherwise it will not work, donno exactly why.
        Button = WidgetTree->ConstructWidget<UButton>();
        WidgetTree->RootWidget = Button;
    }
    return bIsWidgetInitialized;
}

void UDebugCategoryButtonWidget::NativePreConstruct()
{
    Super::NativeConstruct();

    if (!WidgetTree) return;

    HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>();
    CheckBox = WidgetTree->ConstructWidget<UCheckBox>();
    TextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    if (!HorizontalBox || !CheckBox || !TextBlock) return;

    TextBlock->SetShadowOffset(FVector2D(1.0f, 1.0f));
    TextBlock->SetShadowColorAndOpacity(FColor::FromHex("#000000FF"));
    TextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
    CheckBox->SetVisibility(ESlateVisibility::HitTestInvisible);
    CheckBox->SetIsEnabled(false);
    FCheckBoxStyle CheckBoxStyle = CheckBox->GetWidgetStyle();
    CheckBoxStyle.CheckedImage.TintColor = FColor::Black;
    CheckBoxStyle.ForegroundColor = FColor::White;
    CheckBoxStyle.CheckedForeground = FColor::White;
    CheckBoxStyle.HoveredForeground = FColor::White;
    CheckBoxStyle.PressedForeground = FColor::White;
    CheckBoxStyle.UndeterminedForeground = FColor::White;
    CheckBoxStyle.CheckedHoveredForeground = FColor::White;
    CheckBoxStyle.CheckedPressedForeground = FColor::White;
    CheckBox->SetWidgetStyle(CheckBoxStyle);
    HorizontalBox->SetVisibility(ESlateVisibility::HitTestInvisible);

    UButtonSlot* HorizontalBoxButtonSlot = Cast<UButtonSlot>(Button->AddChild(HorizontalBox));
    UHorizontalBoxSlot* CheckboxHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(CheckBox));
    UHorizontalBoxSlot* TextBlockHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(TextBlock));
    if (!HorizontalBoxButtonSlot || !CheckboxHorizontalBoxSlot || !TextBlockHorizontalBoxSlot) return;

    HorizontalBoxButtonSlot->SetHorizontalAlignment(HAlign_Left);
    CheckboxHorizontalBoxSlot->SetPadding(FMargin(0, 0, 10, 0));
    TextBlockHorizontalBoxSlot->SetHorizontalAlignment(HAlign_Left);
    TextBlockHorizontalBoxSlot->SetVerticalAlignment(VAlign_Top);
}

void UDebugCategoryButtonWidget::OnButtonClicked()
{
    if (OnMadeClick.IsBound())
    {
        OnMadeClick.Broadcast(this, UHLDebugCategory.Tags.First());
    }
}

void UDebugCategoryButtonWidget::OnDebugCategoryChanged(FGameplayTag DebugCategoryTag_In, bool bEnabled_In)
{
    if (UHLDebugCategory.Tags.First() == DebugCategoryTag_In)
    {
        UpdateCheckboxState(bEnabled_In);
    }
}
