// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "Core/UHLAIDebugSettings.h"
#include "Core/UHLMathOperations.h"
#include "BTD_RandomChance.generated.h"

UENUM(BlueprintType)
enum class EScalableChanceType : uint8
{
	None,
	ScalableFloat UMETA(ToolTip = "with FScalableFloat value level getted from blackboard ScalableChanceLevelInBB"),
	BBDependantWithSteps UMETA(ToolTip = "Chance changes by math operation that applies every step e.g. step 0 - \"BaseChance\", step 1 - \"BaseChance\" + 20%, step 2 - \"BaseChance\" + 40% with math operation \"Add\" 20%"),
};

/**
 * TODO: blackboard based random chances
 */
UCLASS(Category = "UnrealHelperLibrary")
class UHLAI_API UBTD_RandomChance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_RandomChance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category="Decorator", EditAnywhere)
	EScalableChanceType ScaleType = EScalableChanceType::None;
	
	UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="ScaleType != EScalableChanceType::ScalableFloat", EditConditionHides))
	FValueOrBBKey_Float Chance = 1.0f;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::ScalableFloat", EditConditionHides))
	FScalableFloat ScalableChance;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::ScalableFloat", EditConditionHides))
	FBlackboardKeySelector ScalableChanceLevelInBB;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::BBDependantWithSteps", EditConditionHides))
	FOperationOnBBValue ChanceChangePerStep = {};

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::BBDependantWithSteps", EditConditionHides))
	FBlackboardKeySelector StepsCount;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="ScaleType != EScalableChanceType::None", EditConditionHides))
	FValueOrBBKey_Float MaxChance = 1.0f;

	UPROPERTY(Category="Decorator", EditAnywhere, AdvancedDisplay, meta=(ShowOnlyInnerProperties))
	FUHLAIDebugSettings DebugSettings;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	// TODO value preview?
	// virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

private:
	float GetCurrentChance(UBehaviorTreeComponent& OwnerComp) const;
};
