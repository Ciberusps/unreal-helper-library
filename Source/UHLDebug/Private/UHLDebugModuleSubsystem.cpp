// Pavel Penkov 2025 All Rights Reserved.


#include "UHLDebugModuleSubsystem.h"

#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "UHLGASBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Core/UHLGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugWidget.h"
#include "AA_WaitDebugCategoryChange.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugModuleSubsystem)

UUHLDebugModuleSubsystem::UUHLDebugModuleSubsystem()
{
    UHLDebugWidgetClass = FSoftObjectPath(TEXT("/UHLDebug/UI/UI_UHL_DebugWidget.UI_UHL_DebugWidget_C"));
}

void UUHLDebugModuleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	UAA_WaitDebugCategoryChange* WaitEncounterDebugCategoryChangeTask =
		UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(
			GetWorld(),
			UHLGameplayTags::TAG_UHL_DebugCategory_InputSystem_AbilityInputCache
		);
	WaitEncounterDebugCategoryChangeTask->OnChange.AddUniqueDynamic(
		this,
		&UUHLDebugModuleSubsystem::OnAbilityInputDebugCategoryChanged);
	WaitEncounterDebugCategoryChangeTask->Activate();
}

void UUHLDebugModuleSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UUHLDebugModuleSubsystem::OnAbilityInputDebugCategoryChanged(bool bEnabled)
{
	if (bEnabled)
	{
		UUHLAbilitySystemComponent* UHLASC = GetPlayerAbilitySystemComponent();

		UUHLDebugWidget* DebugWidget = GetOrCreateUHLDebugWidget();
		if (!DebugWidget) return;
		DebugWidget->ToggleAbilityInputCache(UHLASC);	
	}
	else
	{
		if (DebugWidgetInstance)
		{
			DebugWidgetInstance->ToggleAbilityInputCache(nullptr);
		};
	}
}

UUHLDebugWidget* UUHLDebugModuleSubsystem::GetOrCreateUHLDebugWidget()
{
    if (DebugWidgetInstance) return DebugWidgetInstance;

    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    UClass* WidgetClass = UHLDebugWidgetClass.LoadSynchronous();
    DebugWidgetInstance = CreateWidget<UUHLDebugWidget>(PlayerController, WidgetClass);
    DebugWidgetInstance->AddToViewport(99999999);

    return DebugWidgetInstance;
}

APlayerController* UUHLDebugModuleSubsystem::GetPlayerController() const
{
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

UUHLAbilitySystemComponent* UUHLDebugModuleSubsystem::GetPlayerAbilitySystemComponent() const
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
