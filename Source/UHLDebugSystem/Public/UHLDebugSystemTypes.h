// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLDebugSystemTypes.generated.h"

// NOTE: Duplicated from UnrealHelperLibrary to decouple modules.
// Enum values and order must remain consistent for config compatibility.
UENUM(BlueprintType)
enum class EUHLDebugSystemBuildType: uint8
{
	None UMETA(Hidden),
    Editor UMETA(ToolTip="Are you in editor"),

	Debug,
	Development,
	Shipping,
	Test,
};


