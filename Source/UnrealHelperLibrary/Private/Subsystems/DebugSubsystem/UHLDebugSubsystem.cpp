// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

#include "Development/UHLDebugSubsystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/UHLDebugCategoryComponent.h"


UUHLDebugSubsystem::UUHLDebugSubsystem()
    : Super()
{
    // static ConstructorHelpers::FClassFinder<UDebugSubsystemWidget> DefaultMasterClass(TEXT("/Game/Bogatyr/UI/Widgets/UI_DebugSubsystemWidget"));
    // DebugWidgetClass = DefaultMasterClass.Class;
}

void UUHLDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const UUHLDebugSubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugSubsystemSettings>();
    DebugCategories = DeveloperSettings->DebugCategories;

    // TODO on actors initiallized enabled debug categories
    // or give opportunity to make it in PlayerController
    if (DeveloperSettings->bEnableDebugCategoriesOnStart)
    {
        // TODO:
        // GetOuter()->GetWorld()->OnWorldBeginPlay.AddWeakLambda([=]()
        // {
        //     SetUp();
        // });
    }
}

void UUHLDebugSubsystem::SetUp()
{
    if (bSetupped) return;

    bSetupped = true;
    const UUHLDebugSubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugSubsystemSettings>();
    DebugCategories = DeveloperSettings->DebugCategories;

    for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
    {
        if (EnabledDebugCategory.Value == true)
        {
            EnableDebugCategory(EnabledDebugCategory.Key, EnabledDebugCategory.Value);
        }
    };
}

bool UUHLDebugSubsystem::IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const
{
    const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
    {
        return DebugCategory.Tags.HasAny(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory != nullptr)
    {
        return UHLDebugCategory->bEnabled;
    }
    return false;
}

void UUHLDebugSubsystem::EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable)
{
    bool bActivated = bEnable;

    FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
    {
        return DebugCategory.Tags.HasAny(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory != nullptr)
    {
        if (bEnable)
        {
            bActivated = UHLDebugCategory->TryActivate(this);
        }
        else
        {
            // TODO deactivate
        }
    }

    for (FGameplayTag GameplayTag : UHLDebugCategory->Tags.GetGameplayTagArray())
    {
        OnDebugCategoryChanged.Broadcast(GameplayTag, bActivated);
    }
}

APlayerController* UUHLDebugSubsystem::GetPlayerController() const
{
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// void UDebugSubsystem::OnDebugCategoryEnabledInternal(EDebugCategory DebugCategory, bool bEnabled)
// {
//     // if (DebugCategory == EDebugCategory::AI)
//     // {
//     //     UDebugSubsystemWidget* DebugSubsystemWidget = GetOrCreateDebugWidget();
//     //     DebugSubsystemWidget->EnableCombatDebug(bEnabled);
//     // }
// }
