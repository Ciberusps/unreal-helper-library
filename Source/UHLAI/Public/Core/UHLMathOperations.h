#pragma once

#include "UHLBlackboardValueType.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UHLMathOperations.generated.h"

UENUM(BlueprintType)
enum class EUHL_MathOperations : uint8
{
	Set,
	Add,
	Multiply,
	Divide,
};

USTRUCT()
struct FMathOperationResult
{
	EBBValueType CurrentBBKeyValueType;
	int32 IntValue;
	float FloatValue;
	FVector VectorValue;
	FRotator RotatorValue;
};

// struct for easier creating BBValue + BBValue math operations
USTRUCT()
struct FMathOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="CurrentBBKeyValueType==EBBValueType::Int", EditConditionHides))
	EUHL_MathOperations MathOperation = EUHL_MathOperations::Set;

	/** Values */
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Bool", EditConditionHides))
	bool BoolValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Int", EditConditionHides))
	int32 IntValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Float", EditConditionHides))
	float FloatValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::String", EditConditionHides, MultiLine))
	FString StringValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Name", EditConditionHides, MultiLine))
	FName NameValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Vector", EditConditionHides))
	FVector VectorValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Rotator", EditConditionHides))
	FRotator RotatorValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Enum || CurrentBBKeyValueType==EBBValueType::NativeEnum", EditConditionHides, GetOptions="GetEnumOptions"))
	FString EnumStringValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Object", EditConditionHides))
	FBlackboardKeySelector ObjectValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBBValueType::Class", EditConditionHides))
	UClass* ClassValue;
	/** ~Values */

	FString ToString(EBBValueType CurrentBBKeyValueType);

	FMathOperationResult CalculateResult(EBBValueType CurrentBBKeyValueType) const;

	bool IsMathOperationRequired() const {
		return MathOperation == EUHL_MathOperations::Add
			|| MathOperation == EUHL_MathOperations::Multiply
			|| MathOperation == EUHL_MathOperations::Divide;
	};

private:
	
};

// class UHLBlackboardMathOperations
// {
// public:
// 	
// };
