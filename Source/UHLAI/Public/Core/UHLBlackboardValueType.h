// Pavel Penkov 2025 All Rights Reserved.


#pragma once

#include "UHLBlackboardValueType.generated.h"

UENUM()
enum class EBlackboardValueType
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