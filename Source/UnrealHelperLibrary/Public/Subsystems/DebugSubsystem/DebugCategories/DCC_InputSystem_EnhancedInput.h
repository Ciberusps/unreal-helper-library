// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/DebugSubsystem/UHLDebugCategoryComponent.h"
#include "DCC_InputSystem_EnhancedInput.generated.h"

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UDCC_InputSystem_EnhancedInput : public UUHLDebugCategoryComponent
{
	GENERATED_BODY()

public:
    virtual void Activate_Implementation(UObject* ContextObject) override;
    virtual void Deactivate_Implementation(UObject* ContextObject) override;
};
