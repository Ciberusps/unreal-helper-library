#pragma once

#include "UHLBlackboardValueType.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "UHLMathOperations.generated.h"

UENUM(BlueprintType)
enum class EUHL_MathOperations : uint8
{
	None UMETA(Hidden),
	Set,
	Add,
	Multiply,
	Divide,
};

// struct for easier creating BBValue + BBValue math operations
USTRUCT(BlueprintType)
struct FOperationOnBBValue
{
	GENERATED_BODY()

	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Int || CurrentBBKeyValueType==EBlackboardValueType::Float", EditConditionHides))
	EUHL_MathOperations MathOperation = EUHL_MathOperations::Set;

	/** Values */
	// UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Bool", EditConditionHides))
	// bool BoolValue;

	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Int", EditConditionHides))
	FValueOrBBKey_Int32 IntValue;

	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Float", EditConditionHides))
	FValueOrBBKey_Float FloatValue;

	// UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::String", EditConditionHides, MultiLine))
	// FString StringValue;
	// UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Name", EditConditionHides, MultiLine))
	// FName NameValue;

	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Vector", EditConditionHides))
	FValueOrBBKey_Vector VectorValue;

	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Rotator", EditConditionHides))
	FValueOrBBKey_Rotator RotatorValue;
	/** ~Values */

	void SetCurrentBBKeyValueType(EBlackboardValueType CurrentBBKeyValueType_In);
	
	int32 CalculateIntResult(UBehaviorTreeComponent& OwnerComp, int32 CurrentValue) const;
	float CalculateFloatResult(UBehaviorTreeComponent& OwnerComp, float CurrentValue) const;
	FVector CalculateVectorResult(UBehaviorTreeComponent& OwnerComp, FVector CurrentValue) const;
	FRotator CalculateRotatorResult(UBehaviorTreeComponent& OwnerComp, FRotator CurrentValue) const;

	FString ToStringIntValue(FString KeyName) const;
	FString ToStringFloatValue(FString KeyName) const;
	FString ToStringVectorValue(FString KeyName) const;
	FString ToStringRotatorValue(FString KeyName) const;

private:
	UPROPERTY()
	EBlackboardValueType CurrentBBKeyValueType = EBlackboardValueType::None;
};
