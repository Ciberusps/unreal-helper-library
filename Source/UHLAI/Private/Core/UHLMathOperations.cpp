#include "Core/UHLMathOperations.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"


FString FMathOperation::ToString(EBBValueType CurrentBBKeyValueType)
{
	
}

FMathOperationResult FMathOperation::CalculateResult(EBBValueType CurrentBBKeyValueType) const
{
	FMathOperationResult Result = {};
	
	if (IsMathOperationRequired())
	{
		if (CurrentBBKeyValueType == EBBValueType::Int)
		{
			Result.IntValue = MathOperation == EUHL_MathOperations::Set
				? IntValue
				: BlackboardComponent->GetValueAsInt(BlackboardKey.SelectedKeyName);
			if (MathOperation == EUHL_MathOperations::Add)
			{
				Result.IntValue += IntValue;
			}
			else if (MathOperation == EUHL_MathOperations::Multiply)
			{
				Result.IntValue *= IntValue;
			}
			else if (MathOperation == EUHL_MathOperations::Divide)
			{
				Result.IntValue /= IntValue;
			}
		}
		if (CurrentBBKeyValueType == EBBValueType::Float)
		{
			
		}
	}

	return Result;
}