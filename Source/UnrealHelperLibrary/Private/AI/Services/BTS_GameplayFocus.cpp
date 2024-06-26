// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_GameplayFocus.h"

#include "AIController.h"

UBTS_GameplayFocus::UBTS_GameplayFocus(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    NodeName = "Set Gameplay Focus";
    FocusPriority = EAIFocusPriority::Gameplay;
}

#if UE_VERSION_NEWER_THAN(5, 4, 0)
void UBTS_GameplayFocus::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
    InitializeNodeMemory<FBTFocusMemory>(NodeMemory, InitType);
}

void UBTS_GameplayFocus::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
    CleanupNodeMemory<FBTFocusMemory>(NodeMemory, CleanupType);
}
#endif
