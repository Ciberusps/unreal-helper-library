// Pavel Penkov 2025 All Rights Reserved.

#include "UHLAIBlueprintLibrary.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAIBlueprintLibrary)


EBlackboardValueType UUHLAIBlueprintLibrary::BlackboardKeyToBBValueType(FBlackboardKeySelector BlackboardKey)
{
	EBlackboardValueType Result = EBlackboardValueType::None;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		Result = EBlackboardValueType::Bool;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		Result = EBlackboardValueType::Int;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		Result = EBlackboardValueType::Float;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_String::StaticClass())
	{
		Result = EBlackboardValueType::String;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Name::StaticClass())
	{
		Result = EBlackboardValueType::Name;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		Result = EBlackboardValueType::Vector;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		Result = EBlackboardValueType::Rotator;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
	{
		Result = EBlackboardValueType::Enum;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		Result = EBlackboardValueType::NativeEnum;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Result = EBlackboardValueType::Object;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Class::StaticClass())
	{
		Result = EBlackboardValueType::Class;
	}

	return Result;
}