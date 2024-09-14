// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UHLDebugSubsystem.h"

#include "Development/UHLDebugSubsystemSettings.h"
#include "Kismet/GameplayStatics.h"

bool UUHLDebugCategoryComponent::CanActivate_Implementation(UObject* ContextObject) const
{
    return true;
}

void UUHLDebugCategoryComponent::Activate_Implementation(UObject* ContextObject)
{
}

void UUHLDebugCategoryComponent::Deactivate_Implementation(UObject* ContextObject)
{
}

bool FUHLDebugCategory::TryActivate(UObject* ContextObj, APlayerController* PlayerController)
{
    bool bResult = false;

    int32 ComponentsActivated = 0;
    int32 ComponentsRequiredToActivate = Components.Num();
    for (TSubclassOf<UUHLDebugCategoryComponent> ComponentClass : Components)
    {
        if (!ComponentClass)
        {
            ComponentsRequiredToActivate--;
            continue;
        };

        UUHLDebugCategoryComponent* Component = nullptr;
        UUHLDebugCategoryComponent** FoundComponent = InstancedComponents.FindByPredicate([=](UUHLDebugCategoryComponent* DebugCategoryComponent)
        {
            return DebugCategoryComponent->GetClass() == ComponentClass;
        });
        if (!FoundComponent)
        {
            Component = NewObject<UUHLDebugCategoryComponent>(ContextObj, ComponentClass);
            InstancedComponents.Add(Component);
        }
        else
        {
            Component = *FoundComponent;
        }
        if (Component->CanActivate(ContextObj))
        {
            Component->Activate(ContextObj);
            ComponentsActivated++;
        }
    }
    if (ComponentsActivated == ComponentsRequiredToActivate)
    {
        bResult = true;
    }

    return bResult;
}

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
    if (DeveloperSettings->bEnabledDebugCategoriesOnStart)
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
    return false;
    // const bool* bIsEnabled = EnabledDebugCategories.Find(DebugCategoryTag);
    // return bIsEnabled != nullptr && *bIsEnabled;
}

void UUHLDebugSubsystem::EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bIsEnabled)
{
    bool bActivated = bIsEnabled;

    FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory DebugCategory)
    {
        return DebugCategory.Tags.HasAny(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory != nullptr)
    {
        bActivated = UHLDebugCategory->TryActivate(this, GetPlayerController());
    }

    // EnabledDebugCategories.Add(DebugCategoryTag);
    // EnabledDebugCategories[DebugCategoryTag] = bIsEnabled;
    OnDebugCategoryEnabled.Broadcast(DebugCategoryTag, bActivated);
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
