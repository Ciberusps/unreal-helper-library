// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_ClearFocus.h"
#include "AIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_ClearFocus)

UBTT_ClearFocus::UBTT_ClearFocus(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NodeName = "ClearFocus";
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TObjectPtr<AAIController> AIOwner = OwnerComp.GetAIOwner();
    if (!AIOwner.Get())
    {
        AIOwner.Get()->ClearFocus(EAIFocusPriority::Gameplay);
    }

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}
