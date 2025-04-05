// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_TimeLimit.h"
#include "Misc/EngineVersionComparison.h"
#include "BehaviorTree/BTDecorator.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Misc/EngineVersionComparison.h"
#include "BTD_TimeLimitRandom.generated.h"


struct FBTimeLimitMemoryRandom
{
};

/**
 * In UE5.4 changed completely now we can't nest from "TimeLimit" and forced to copy-paste same functionality
 */
UCLASS(Category = "UnrealHelperLibrary", hidecategories="Decorator")
class UHLAI_API UBTD_TimeLimitRandom : public UBTDecorator
{
	GENERATED_BODY()

public:
    UBTD_TimeLimitRandom(const FObjectInitializer& ObjectInitializer);

	/** TimeLimit */
	UPROPERTY(EditAnywhere, Category="TimeLimitRandom", meta=(ClampMin="0"))
	FFloatInterval TimeLimitRange = FFloatInterval(3.0f, 5.0f);
	UPROPERTY(EditAnywhere, Category="TimeLimitRandom")
	bool bUseIntegers = true;

    virtual FString GetStaticDescription() const override;

#if UE_VERSION_NEWER_THAN(5, 4, 0)
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
#if WITH_EDITOR
    virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
#endif

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

#if UE_VERSION_NEWER_THAN(5, 4, 0)
    // 5.4.0 and up only code
    virtual uint16 GetInstanceMemorySize() const override;
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
#endif

private:
    /** max allowed time for execution of underlying node */
    UPROPERTY(EditAnywhere, Category="Decorator")
    float TimeLimit;
};
