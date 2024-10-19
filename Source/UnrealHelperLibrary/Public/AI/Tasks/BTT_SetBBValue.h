// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"
#include "BTT_SetBBValue.generated.h"

UENUM(BlueprintType)
enum class ESetBBValue_MathOperations : uint8
{
    Set,
    Add,
    Multiply,
    Divide,
};

/**
 * Use with caution, setting BB values in BehaviorTrees directly is "anti-pattern"
 * mostly you want to write BTTasks where its possible, use it for test purposes
 *
 * TODO arithmetic operations like A + B, A * B ...
 * TODO vector calculations - add relative to owner, e.g. set Value as GetActorLocation + Value(offset)
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTT_SetBBValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetBBValue(const FObjectInitializer& ObjectInitializer);

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	struct FBlackboardKeySelector BlackboardKey;

    // TODO:
    // UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="CurrentBBKeyValueType==EBBValueType::Int"))
    // bool bDoMath = false;
    // UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="bDoMath && CurrentBBKeyValueType==EBBValueType::Int", EditConditionHides))
    UPROPERTY(Category="Blackboard", EditAnywhere, meta=(EditCondition="CurrentBBKeyValueType==EBBValueType::Int", EditConditionHides))
    ESetBBValue_MathOperations MathOperation = ESetBBValue_MathOperations::Set;

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

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

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
	EBBValueType CurrentBBKeyValueType = EBBValueType::None;

    UFUNCTION()
    TArray<FString> GetEnumOptions();
    bool IsMathOperationRequired() const {
        return MathOperation == ESetBBValue_MathOperations::Add
            || MathOperation == ESetBBValue_MathOperations::Multiply
            || MathOperation == ESetBBValue_MathOperations::Divide;
    };
};
