// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_RandomChance.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTD_RandomChance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_RandomChance(const FObjectInitializer& ObjectInitializer);

	// UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="!bUseUnclamped && !bUseBlackboardValue", EditConditionHides))
	UPROPERTY(Category="Decorator", EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f))
	float Chance = 1.0f;
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

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
	// TODO value preview?
	// virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

};
