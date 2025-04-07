// Pavel Penkov 2025 All Rights Reserved.


#include "UHLDebugUISubsystem.h"

#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Blueprint/UserWidget.h"
#include "Core/UHLGameplayTags.h"
#include "Development/UHLDebugSubsystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugWidget.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugSubsystem)

UUHLDebugUISubsystem::UUHLDebugUISubsystem()
{
    UHLDebugWidgetClass = FSoftObjectPath(TEXT("/UnrealHelperLibrary/UI/UI_UHL_DebugWidget.UI_UHL_DebugWidget_C"));
}

void UUHLDebugUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	SetUp();
}

void UUHLDebugUISubsystem::Deinitialize()
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

void UUHLDebugUISubsystem::SetUp()
{
    if (bSetupped) return;
    bSetupped = true;

    const UUHLDebugUISubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugUISubsystemSettings>();
    DebugCategories = DeveloperSettings->DebugCategories;

    OnDebugCategoryChanged.AddUniqueDynamic(this, &UUHLDebugUISubsystem::OnDebugCategoryChangedInternal);

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

void UUHLDebugUISubsystem::SetUpCategoriesThatRequiresPlayerController()
{
    if (bSetUpCategoriesThatRequiresPlayerController) return;
    bSetUpCategoriesThatRequiresPlayerController = true;

    const UUHLDebugUISubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugUISubsystemSettings>();

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

void UUHLDebugUISubsystem::ToggleAbilityInputDebug()
{
    UUHLAbilitySystemComponent* UHLASC = GetPlayerAbilitySystemComponent();

    UUHLDebugWidget* DebugWidget = GetOrCreateUHLDebugWidget();
    if (!DebugWidget) return;
    DebugWidget->ToggleAbilityInputCache(UHLASC);
}

UUHLDebugWidget* UUHLDebugUISubsystem::GetOrCreateUHLDebugWidget()
{
    if (DebugWidgetInstance) return DebugWidgetInstance;

    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    const UUHLDebugUISubsystemSettings* DeveloperSettings = GetDefault<UUHLDebugUISubsystemSettings>();
    DebugCategories = DeveloperSettings->DebugCategories;

    UClass* WidgetClass = UHLDebugWidgetClass.LoadSynchronous();
    DebugWidgetInstance = CreateWidget<UUHLDebugWidget>(PlayerController, WidgetClass);
    DebugWidgetInstance->AddToViewport(99999999);

    return DebugWidgetInstance;
}

void UUHLDebugUISubsystem::OnDebugCategoryChangedInternal(FGameplayTag DebugCategoryTag, bool bEnabled)
{
    if (DebugCategoryTag == UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_AbilityInputCache)
    {
        ToggleAbilityInputDebug();
    }
}

APlayerController* UUHLDebugUISubsystem::GetPlayerController() const
{
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

UUHLAbilitySystemComponent* UUHLDebugUISubsystem::GetPlayerAbilitySystemComponent() const
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
