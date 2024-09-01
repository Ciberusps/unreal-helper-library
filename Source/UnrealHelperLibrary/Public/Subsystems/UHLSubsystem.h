// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/UHLDebugWidget.h"
#include "UHLSubsystem.generated.h"


/**
 * Draft of UHLSubsystem - should toggle widgets on UHLDebugWidget
 * TODO:
 * - refactor
 */
UCLASS()
class UNREALHELPERLIBRARY_API UUHLSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    UUHLSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(Exec, BlueprintCallable)
    void ToggleAbilityInputDebug();

    UFUNCTION(Exec, BlueprintCallable)
    UUHLDebugWidget* GetOrCreateUHLDebugWidget();

private:
    UPROPERTY()
    TSubclassOf<UUHLDebugWidget> UHLDebugWidgetClass;
    UPROPERTY()
    UUHLDebugWidget* DebugWidgetInstance = nullptr;

    APlayerController* GetPlayerController() const;
    UUHLAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;
};
