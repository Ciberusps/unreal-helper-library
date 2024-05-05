// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_RandomChance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UBTD_RandomChance : public UBTDecorator
{
	GENERATED_BODY()

	UBTD_RandomChance(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(UIMin=0.0f, UIMax=1.0f, ClampMin=0.0f, ClampMax=1.0f, EditCondition="!bUseUnclamped && !bUseBlackboardValue", EditConditionHides))
	float Chance;
	// UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="bUseUnclamped && !bUseBlackboardValue", EditConditionHides))
	// float ChanceUnclamped;
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="bUseBlackboardValue", EditConditionHides))
	FBlackboardKeySelector ChanceInBB;

	// UPROPERTY(Category=Decorator, EditAnywhere)
	// bool bUseUnclamped = false;
	UPROPERTY(Category=Decorator, EditAnywhere)
	bool bUseBlackboardValue = false;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
	// TODO value preview?
	// virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

};
