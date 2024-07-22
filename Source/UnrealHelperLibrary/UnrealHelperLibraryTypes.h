#pragma once

const FVector VECTOR_ERROR = FVector(404, 404, 404);
const float FLOAT_ERROR = -99999.0f;

UENUM()
enum class EBBValueType
{
	None,
	Bool,
	Int,
	Float,
	String,
	Name,
	Vector,
	Rotator,
	Enum,
	NativeEnum,
	Object,
	Class,
};

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
