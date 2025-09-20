// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/UHLDebugWidget.h"
#include "UHLDebugModuleSubsystem.generated.h"

// TODO use ULocalPlayer
UCLASS()
class UHLDEBUG_API UUHLDebugModuleSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
    UUHLDebugModuleSubsystem();

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    UPROPERTY()
    TSoftClassPtr<UUHLDebugWidget> UHLDebugWidgetClass;
    UPROPERTY()
    UUHLDebugWidget* DebugWidgetInstance = nullptr;

	// TODO use ULocalPlayer
    APlayerController* GetPlayerController() const;
	UUHLAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;
	UUHLDebugWidget* GetOrCreateUHLDebugWidget();

	UFUNCTION()
    void OnAbilityInputDebugCategoryChanged(bool bEnabled);
};
