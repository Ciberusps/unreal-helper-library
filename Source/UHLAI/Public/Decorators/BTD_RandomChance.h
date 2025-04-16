// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTD_RandomChance.generated.h"

UENUM(BlueprintType)
enum class EScalableChanceType : uint8
{
	None,
	ScalableFloat,
	BBDependantWithSteps,
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

	// UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="!bUseUnclamped && !bUseBlackboardValue", EditConditionHides))
	UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="ScaleType != EScalableChanceType::None", EditConditionHides))
	FValueOrBBKey_Float Chance = 1.0f;

	// UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="bUseUnclamped && !bUseBlackboardValue", EditConditionHides))
	// float ChanceUnclamped;
    // TODO value from blackboard
	// UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="bUseBlackboardValue", EditConditionHides))
	// FBlackboardKeySelector ChanceInBB;

	// UPROPERTY(Category=Decorator, EditAnywhere)
	// bool bUseUnclamped = false;
    // TODO value from blackboard
	// UPROPERTY(Category=Decorator, EditAnywhere)
	// bool bUseBlackboardValue = false;

	UPROPERTY(Category="Decorator", EditAnywhere)
	EScalableChanceType ScaleType = EScalableChanceType::None;
	
	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::ScalableFloat", EditConditionHides))
	FScalableFloat ScalableChanceValue = false;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::ScalableFloat", EditConditionHides))
	FBlackboardKeySelector ScalableChanceValueLevelFromBB;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="ScaleType == EScalableChanceType::ScalableFloat", EditConditionHides))
	float ChanceChangePerStep = false;

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="ScaleType != EScalableChanceType::None", EditConditionHides))
	FValueOrBBKey_Float MaxChance = 1.0f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
	// TODO value preview?
	// virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

private:
	float GetCurrentChance(UBehaviorTreeComponent& OwnerComp);
};
