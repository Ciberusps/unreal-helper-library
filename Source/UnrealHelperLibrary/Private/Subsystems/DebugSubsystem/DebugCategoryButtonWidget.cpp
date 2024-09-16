// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/DebugCategoryButtonWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CheckBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/UHLDebugCategory.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

void UDebugCategoryButtonWidget::SetUp(const FUHLDebugCategory& UHLDebugCategory_In)
{
    UHLDebugCategory = UHLDebugCategory_In;
    TextBlock->SetText(FText::FromString(UHLDebugCategory_In.ShortName));
    UpdateCheckboxState(UHLDebugCategory_In.bEnabled);
    Button->SetBackgroundColor(UHLDebugCategory.Color.Desaturate(0.6f));
}

void UDebugCategoryButtonWidget::UpdateCheckboxState(bool bEnabled_In)
{
    CheckBox->SetCheckedState(bEnabled_In ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

bool UDebugCategoryButtonWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    // more correct way to avoid error. than in original post.
    if(!HasAnyFlags(RF_ClassDefaultObject) )
    {
        // root have to be initialized in Initialize function, otherwise it will not work, donno exactly why.
        Button = WidgetTree->ConstructWidget<UButton>();
        WidgetTree->RootWidget = Button;
    }
    return bIsWidgetInitialized;
}

void UDebugCategoryButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSubsystem>();
    Button->OnClicked.AddUniqueDynamic(this, &UDebugCategoryButtonWidget::OnButtonClicked);

    HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>();
    CheckBox = WidgetTree->ConstructWidget<UCheckBox>();
    TextBlock = WidgetTree->ConstructWidget<UTextBlock>();

    TextBlock->SetShadowOffset(FVector2D(1.0f, 1.0f));
    TextBlock->SetShadowColorAndOpacity(FColor::FromHex("#000000FF"));

    UButtonSlot* HorizontalBoxButtonSlot = Cast<UButtonSlot>(Button->AddChild(HorizontalBox));
    UHorizontalBoxSlot* CheckboxHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(CheckBox));
    UHorizontalBoxSlot* TextBlockHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(TextBlock));

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
