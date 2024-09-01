// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UHLSubsystem.h"

#include "AbilitySystemInterface.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugWidget.h"

UUHLSubsystem::UUHLSubsystem()
    : Super()
{
    static ConstructorHelpers::FClassFinder<UUHLDebugWidget> DefaultMasterClass(TEXT("/UnrealHelperLibrary/UI/UI_UHL_DebugWidget"));
    UHLDebugWidgetClass = DefaultMasterClass.Class;
}

void UUHLSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UUHLSubsystem::ToggleAbilityInputDebug()
{
    UUHLAbilitySystemComponent* UHLASC = GetPlayerAbilitySystemComponent();

    UUHLDebugWidget* DebugWidget = GetOrCreateUHLDebugWidget();
    if (!DebugWidget) return;
    DebugWidget->ToggleAbilityInputCache(UHLASC);
}

UUHLDebugWidget* UUHLSubsystem::GetOrCreateUHLDebugWidget()
{
    if (DebugWidgetInstance) return DebugWidgetInstance;

    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    DebugWidgetInstance = CreateWidget<UUHLDebugWidget>(PlayerController, UHLDebugWidgetClass);
    DebugWidgetInstance->AddToViewport(99999999);

    return DebugWidgetInstance;
}

APlayerController* UUHLSubsystem::GetPlayerController() const
{
    // TArray<AActor*> FoundActors;
    // UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UAbilitySystemInterface::StaticClass(), FoundActors);
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

UUHLAbilitySystemComponent* UUHLSubsystem::GetPlayerAbilitySystemComponent() const
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
