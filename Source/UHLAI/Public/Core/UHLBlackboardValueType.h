#pragma once

#include "UHLBlackboardValueType.generated.h"

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