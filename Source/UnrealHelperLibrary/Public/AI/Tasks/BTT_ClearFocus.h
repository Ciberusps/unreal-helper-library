// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearFocus.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTT_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_ClearFocus(const FObjectInitializer& ObjectInitializer);

    // not possible to expose "EAIFocusPriority::Type"
    // by default clears Gameplay level priority
    // UPROPERTY(Category="Decorator", EditAnywhere)
    // EAIFocusPriority::Type Priority = (uint8)EAIFocusPriority::Gameplay;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
