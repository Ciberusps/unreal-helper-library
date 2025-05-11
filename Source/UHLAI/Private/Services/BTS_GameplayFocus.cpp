// Pavel Penkov 2025 All Rights Reserved.


#include "Services/BTS_GameplayFocus.h"

#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "AIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTS_GameplayFocus)

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
