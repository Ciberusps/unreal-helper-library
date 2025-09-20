// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"  // This includes FVector definition
#include "UnrealHelperLibraryTypes.generated.h"  // Required for UENUM macros

const FVector VECTOR_ERROR = FVector(404.0f, 404.0f, 404.0f);
const float FLOAT_ERROR = -99999.0f;


UENUM(BlueprintType)
enum class EUHLDirection: uint8
{
    None,

    Front,
    Back,
    Left,
    Right,

    FrontLeft,
    FrontRight,
    BackLeft,
    BackRight,

    MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EUHLSettingsSource: uint8
{
    None UMETA(Hidden),

    Node,
    DataAsset,
    Actor,

    MAX UMETA(Hidden)
};

// EUHLBuildType not same as EBuildConfiguration, if we WITH_EDITOR returns Editor,
// no matter what EBuildConfiguration is currently used(mostly WITH_EDITOR is Development)
// if real build returns EBuildConfiguration
UENUM(BlueprintType)
enum class EUHLBuildType: uint8
{
    None UMETA(Hidden),
    Editor UMETA(ToolTip="Are you in editor"),

    Debug,
    Development,
    Shipping,
    Test,
};
