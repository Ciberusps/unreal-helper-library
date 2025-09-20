// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Core/UHLBlackboardValueType.h"
#include "Core/UHLMathOperations.h"
#include "BTT_SetBBValue.generated.h"

/**
 * Use with caution, setting BB values in BehaviorTrees directly is "anti-pattern"
 * mostly you want to write BTTasks where its possible, use it for test purposes
 *
 * TODO arithmetic operations like A + B, A * B ...
 * TODO vector calculations - add relative to owner, e.g. set Value as GetActorLocation + Value(offset)
 */
UCLASS(Category = "UnrealHelperLibrary")
class UHLAI_API UBTT_SetBBValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetBBValue(const FObjectInitializer& ObjectInitializer);

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	struct FBlackboardKeySelector BlackboardKey;

	UPROPERTY(
		EditAnywhere, Category="Blackboard",
		meta=(EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Int || CurrentBBKeyValueType==EBlackboardValueType::Float || CurrentBBKeyValueType==EBlackboardValueType::Vector || CurrentBBKeyValueType==EBlackboardValueType::Rotator",
			EditConditionHides, ShowOnlyInnerProperties))
	FOperationOnBBValue OperationOnBBValue;

	/** Values */
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Bool", EditConditionHides))
	bool BoolValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::String", EditConditionHides, MultiLine))
	FString StringValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Name", EditConditionHides, MultiLine))
	FName NameValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Enum || CurrentBBKeyValueType==EBlackboardValueType::NativeEnum", EditConditionHides, GetOptions="GetEnumOptions"))
	FString EnumStringValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Object", EditConditionHides))
	FBlackboardKeySelector ObjectValue;
	UPROPERTY(Category="Blackboard", EditAnywhere, meta=(DisplayName="Value", EditCondition="CurrentBBKeyValueType==EBlackboardValueType::Class", EditConditionHides))
	UClass* ClassValue;
	/** ~Values */

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void PostLoad() override;

#if WITH_EDITORONLY_DATA

	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;
	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<EArithmeticKeyOperation::Type> ArithmeticOperation;
	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<ETextKeyOperation::Type> TextOperation;

#endif

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY()
	UEnum* CurrentEnum;
	UPROPERTY()
	EBlackboardValueType CurrentBBKeyValueType = EBlackboardValueType::None;

    UFUNCTION()
    TArray<FString> GetEnumOptions();

	UPROPERTY(meta=(DeprecatedProperty, DeprecationMessage="Use \"MathOperationNew\" instead"))
	EUHL_MathOperations MathOperation = EUHL_MathOperations::None;
	UPROPERTY(meta=(DeprecatedProperty, DeprecationMessage="Use \"OperationOnBBValue.IntValue\" instead"))
	int32 IntValue;
	UPROPERTY(meta=(DeprecatedProperty, DeprecationMessage="Use \"OperationOnBBValue.FloatValue\" instead"))
	float FloatValue;
	UPROPERTY(meta=(DeprecatedProperty, DeprecationMessage="Use \"OperationOnBBValue.VectorValue\" instead"))
	FVector VectorValue;
	UPROPERTY(meta=(DeprecatedProperty, DeprecationMessage="Use \"OperationOnBBValue.RotatorValue\" instead"))
	FRotator RotatorValue;

	UPROPERTY()
	bool bMigratedToMathOperation = false;
};
