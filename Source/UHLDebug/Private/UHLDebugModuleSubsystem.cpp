// Pavel Penkov 2025 All Rights Reserved.


#include "UHLDebugModuleSubsystem.h"

#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugWidget.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "HAL/IConsoleManager.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugModuleSubsystem)

UUHLDebugModuleSubsystem::UUHLDebugModuleSubsystem()
{
    UHLDebugWidgetClass = FSoftObjectPath(TEXT("/UnrealHelperLibrary/UI/UI_UHL_DebugWidget.UI_UHL_DebugWidget_C"));
}

void UUHLDebugModuleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	// Console variable to control ability input cache debug
	static TAutoConsoleVariable<int32> GUHLAbilityInputCacheCVar(
		TEXT("uhl.Debug.AbilityInputCache"),
		0,
		TEXT("Toggle UHL ability input cache debug widget (0/1)"),
		ECVF_Default);

	// Install a console variable sink to listen to changes
	ConsoleVariableSinkHandle = IConsoleManager::Get().RegisterConsoleVariableSink_Handle(FConsoleCommandDelegate::CreateUObject(
		this,
		&UUHLDebugModuleSubsystem::OnCVarUHLAbilityInputCacheChanged));

	// Apply initial value
	OnCVarUHLAbilityInputCacheChanged();
}

void UUHLDebugModuleSubsystem::Deinitialize()
{
    Super::Deinitialize();
	IConsoleManager::Get().UnregisterConsoleVariableSink_Handle(ConsoleVariableSinkHandle);
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

void UUHLDebugModuleSubsystem::OnCVarUHLAbilityInputCacheChanged()
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("uhl.Debug.AbilityInputCache"));
	const bool bEnabled = Var ? Var->GetInt() != 0 : false;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			bEnabled ? FColor::Green : FColor::Silver,
			FString::Printf(TEXT("[UHLDebug] AbilityInputCache %s"), bEnabled ? TEXT("ON") : TEXT("OFF"))
		);
	}

	UE_LOG(LogTemp, Log, TEXT("[UHLDebug] AbilityInputCache %s"), bEnabled ? TEXT("ON") : TEXT("OFF"));

	OnAbilityInputDebugCategoryChanged(bEnabled);
}
