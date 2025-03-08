// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTD_LoopRandomCount.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary", hidecategories="Decorator")
class UNREALHELPERLIBRARY_API UBTD_LoopRandomCount : public UBTDecorator_Loop
{
	GENERATED_BODY()

public:
	UBTD_LoopRandomCount(const FObjectInitializer& ObjectInitializer);

	/** number of executions */
	UPROPERTY(EditAnywhere, Category="LoopRandomCount", meta=(EditCondition="!bInfiniteLoop", ClampMin="1", ClampMax="255"))
	FInt32Interval Range = FInt32Interval(2, 4);

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	virtual FString GetStaticDescription() const override;

};
