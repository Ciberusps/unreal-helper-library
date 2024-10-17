// Copyright (c) 2024 Pavel Penkov


#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Core/UHLGameplayTags.h"
#include "Development/UHLDebugSubsystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugWidget.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugSubsystem)

UUHLDebugSubsystem::UUHLDebugSubsystem()
{
    UHLDebugWidgetClass = FSoftObjectPath(TEXT("/UnrealHelperLibrary/UI/UI_UHL_DebugWidget.UI_UHL_DebugWidget_C"));
}

void UUHLDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	SetUp();
}

void UUHLDebugSubsystem::Deinitialize()
{
    for (FUHLDebugCategory& DebugCategory : DebugCategories)
    {
        // if (DebugCategory.bForceComponentsDeactivateOnEnd)
        if (DebugCategory.GetIsEnabled())
        {
            DebugCategory.TryDisable(this);
        }
    }
    Super::Deinitialize();
}

void UUHLDebugSubsystem::SetUp()
{
    if (bSetupped) return;
    bSetupped = true;

    const UUHLDebugSubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugSubsystemSettings>();
    DebugCategories = DeveloperSettings->DebugCategories;

    OnDebugCategoryChanged.AddUniqueDynamic(this, &UUHLDebugSubsystem::OnDebugCategoryChangedInternal);

    for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
    {
        const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
		{
			return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
		});
        if (EnabledDebugCategory.Value == true && UHLDebugCategory != nullptr && !UHLDebugCategory->bRequiresPlayerControllerToEnable)
        {
            EnableDebugCategory(EnabledDebugCategory.Key, EnabledDebugCategory.Value);
        }
    };

    EUHLBuildType BuildType = UUnrealHelperLibraryBPL::GetBuildType();
    if (BuildType != EUHLBuildType::Editor)
    {
        for (const FUHLDebugCategory& DebugCategory : DebugCategories)
        {
            if (DebugCategory.ByDefaultEnabledInBuildTypes.Contains(BuildType))
            {
                EnableDebugCategory(DebugCategory.Tags.First(), true);
            }
        }
    }
}

void UUHLDebugSubsystem::SetUpCategoriesThatRequiresPlayerController()
{
    if (bSetUpCategoriesThatRequiresPlayerController) return;
    bSetUpCategoriesThatRequiresPlayerController = true;

    const UUHLDebugSubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugSubsystemSettings>();

    for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
    {
        const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
        {
            return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
        });
        if (EnabledDebugCategory.Value == true && UHLDebugCategory != nullptr && UHLDebugCategory->bRequiresPlayerControllerToEnable)
        {
            EnableDebugCategory(EnabledDebugCategory.Key, EnabledDebugCategory.Value);
        }
    };
    bIsSetuping = false;
}

bool UUHLDebugSubsystem::IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const
{
    const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
    {
        return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory != nullptr)
    {
        return UHLDebugCategory->GetIsEnabled();
    }
    return false;
}

void UUHLDebugSubsystem::EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable)
{
    bool bEnabled = bEnable;

    FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
    {
        return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory == nullptr) return;
    if (UHLDebugCategory->bRequiresPlayerControllerToEnable && !bSetUpCategoriesThatRequiresPlayerController) return;
    if (UHLDebugCategory->GetIsEnabled() && bEnable) return;
    if (!UHLDebugCategory->GetIsEnabled() && !bEnable) return;

    if (bEnable)
    {
        // Disable blocked DebugCategories
        for (const FUHLDebugCategory& DebugCategory : DebugCategories)
        {
            if (DebugCategory != *UHLDebugCategory
                && DebugCategory.Tags.HasAny(UHLDebugCategory->Blocks))
            {
                EnableDebugCategory(DebugCategory.Tags.First(), false);
            }
        }

        bEnabled = UHLDebugCategory->TryEnable(this);
    }
    else
    {
        if (!bIsSetuping)
        // TODO remove
            // || (bIsSetuping && UHLDebugCategory->bForceComponentsDeactivateOnEnd))
        {
            UHLDebugCategory->TryDisable(this);
        }
    }

    for (FGameplayTag GameplayTag : UHLDebugCategory->Tags.GetGameplayTagArray())
    {
        OnDebugCategoryChanged.Broadcast(GameplayTag, bEnabled);
    }
}

void UUHLDebugSubsystem::ToggleDebugCategory(const FGameplayTag DebugCategoryTag)
{
    EnableDebugCategory(DebugCategoryTag, !IsCategoryEnabled(DebugCategoryTag));
}

const FUHLDebugCategory& UUHLDebugSubsystem::GetDebugCategoryByTag(const FGameplayTag DebugCategoryTag) const
{
	const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
	{
		return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(DebugCategoryTag));
	});
	return *UHLDebugCategory;
}

void UUHLDebugSubsystem::ToggleAbilityInputDebug()
{
    UUHLAbilitySystemComponent* UHLASC = GetPlayerAbilitySystemComponent();

    UUHLDebugWidget* DebugWidget = GetOrCreateUHLDebugWidget();
    if (!DebugWidget) return;
    DebugWidget->ToggleAbilityInputCache(UHLASC);
}

UUHLDebugWidget* UUHLDebugSubsystem::GetOrCreateUHLDebugWidget()
{
    if (DebugWidgetInstance) return DebugWidgetInstance;

    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    const UUHLDebugSubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugSubsystemSettings>();
    DebugCategories = DeveloperSettings->DebugCategories;

    UClass* WidgetClass = UHLDebugWidgetClass.LoadSynchronous();
    DebugWidgetInstance = CreateWidget<UUHLDebugWidget>(PlayerController, WidgetClass);
    DebugWidgetInstance->AddToViewport(99999999);

    return DebugWidgetInstance;
}

void UUHLDebugSubsystem::OnDebugCategoryChangedInternal(FGameplayTag DebugCategoryTag, bool bEnabled)
{
    if (DebugCategoryTag == UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_AbilityInputCache)
    {
        ToggleAbilityInputDebug();
    }
}

APlayerController* UUHLDebugSubsystem::GetPlayerController() const
{
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

UUHLAbilitySystemComponent* UUHLDebugSubsystem::GetPlayerAbilitySystemComponent() const
{
    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn) return nullptr;

    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Pawn);
    if (!AbilitySystemInterface) return nullptr;

    UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
    if (!ASC) return nullptr;

    UUHLAbilitySystemComponent* UHLASC = Cast<UUHLAbilitySystemComponent>(ASC);
    if (!UHLASC) return nullptr;

    return UHLASC;
}
