// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTDecorator_LoopRandomCount.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary", hidecategories="Decorator")
class UNREALHELPERLIBRARY_API UBTDecorator_LoopRandomCount : public UBTDecorator_Loop
{
	GENERATED_BODY()

public:
	UBTDecorator_LoopRandomCount(const FObjectInitializer& ObjectInitializer);

	/** number of executions */
	UPROPERTY(EditAnywhere, meta=(EditCondition="!bInfiniteLoop", ClampMin="1", ClampMax="255"))
	FInt32Interval Range = FInt32Interval(2, 4);

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

};
