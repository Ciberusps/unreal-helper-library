// Pavel Penkov 2025 All Rights Reserved.


#include "UI/UHLDebugWidget.h"

#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugWidget)

void UUHLDebugWidget::ToggleAbilityInputCache(UUHLAbilitySystemComponent* ASC_In)
{
    ASC = ASC_In;
    if (ASC.Get())
    {
        bShowAbilityInputCacheList = !bShowAbilityInputCacheList;
    }
}

void UUHLDebugWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bShowAbilityInputCacheList)
    {
        FString NewText = "";
        TArray<FGameplayTag> AbilityInputCache = ASC->GetAbilityInputCache()->GetAbilityInputCache();

        for (int32 i = AbilityInputCache.Num() - 1; i >= 0; i--)
        {
            NewText.Append(AbilityInputCache[i].ToString() + FString("\n"));
        }

        AbilityInputCacheTextBlock->SetText(FText::FromString(NewText));
    }
}
