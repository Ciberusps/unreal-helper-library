// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "UHLDebugWidget.generated.h"


class UTextBlock;

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UUHLDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UHLDebugWidget", meta = (BindWidget))
    UTextBlock* AbilityInputCacheTextBlock;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION(BlueprintCallable, Category="UHLDebugWidget")
    void ToggleAbilityInputCache(UUHLAbilitySystemComponent* ASC_In);

private:
    bool bShowAbilityInputCacheList = false;
    TWeakObjectPtr<UUHLAbilitySystemComponent> ASC;
};
