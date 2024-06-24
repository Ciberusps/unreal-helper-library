#pragma once

const FVector VECTOR_ERROR = FVector(404, 404, 404);

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
