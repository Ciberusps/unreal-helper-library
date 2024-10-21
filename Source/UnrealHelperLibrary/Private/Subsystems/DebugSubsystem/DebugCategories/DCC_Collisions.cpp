// Copyright (c) 2024 Pavel Penkov


#include "Subsystems/DebugSubsystem/DebugCategories/DCC_Collisions.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_Collisions)

void UDCC_Collisions::Activate_Implementation(UObject* ContextObject)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("ShowFlag.Collision 1"));
    Super::Activate_Implementation(ContextObject);
}

void UDCC_Collisions::Deactivate_Implementation(UObject* ContextObject)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(ContextObject->GetWorld(), FString("ShowFlag.Collision 0"));
    Super::Deactivate_Implementation(ContextObject);
}
