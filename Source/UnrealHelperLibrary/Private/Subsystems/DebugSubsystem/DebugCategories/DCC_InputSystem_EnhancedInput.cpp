// Copyright (c) 2024 Pavel Penkov


#include "Subsystems/DebugSubsystem/DebugCategories/DCC_InputSystem_EnhancedInput.h"

#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_InputSystem_EnhancedInput)

void UDCC_InputSystem_EnhancedInput::Activate_Implementation(UObject* ContextObject)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("showdebug enhancedinput"));
    Super::Activate_Implementation(ContextObject);
}

void UDCC_InputSystem_EnhancedInput::Deactivate_Implementation(UObject* ContextObject)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("showdebug reset"));
    Super::Deactivate_Implementation(ContextObject);
}
