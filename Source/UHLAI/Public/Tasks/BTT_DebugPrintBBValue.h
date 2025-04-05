// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DebugPrintBBValue.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary")
class UHLAI_API UBTT_DebugPrintBBValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DebugPrintBBValue(const FObjectInitializer& ObjectInitializer);

	// UPROPERTY(Category=Decorator, EditAnywhere)
	// FString Line1;
	UPROPERTY(Category="Decorator", EditAnywhere)
	FBlackboardKeySelector BlackboardKey;
	UPROPERTY(Category="Decorator", EditAnywhere)
	float Duration = 3.5f;
    UPROPERTY(Category="Decorator", EditAnywhere)
    FName Key = NAME_None;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FString GetBBKeyDescription(UBehaviorTreeComponent& OwnerComp);
	virtual FString GetStaticDescription() const override;
};
