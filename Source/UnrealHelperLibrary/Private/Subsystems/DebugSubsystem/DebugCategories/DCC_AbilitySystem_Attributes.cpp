// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/DebugCategories/DCC_AbilitySystem_Attributes.h"

#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_AbilitySystem_Attributes)

void UDCC_AbilitySystem_Attributes::Activate_Implementation(UObject* ContextObject)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("showdebug abilitysystem"));
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("AbilitySystem.Debug.SetCategory Attributes"));
    return Super::Activate_Implementation(ContextObject);
}

void UDCC_AbilitySystem_Attributes::Deactivate_Implementation(UObject* ContextObject)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("showdebug reset"));
    Super::Deactivate_Implementation(ContextObject);
}
