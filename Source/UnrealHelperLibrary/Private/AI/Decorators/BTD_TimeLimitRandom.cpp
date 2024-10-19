// Copyright (c) 2024 Pavel Penkov


#include "AI/Decorators/BTD_TimeLimitRandom.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_TimeLimitRandom)

#if UE_VERSION_NEWER_THAN(5, 4, 0)
struct FBTimeLimitRandomMemory
{
    bool bElapsed = false;
};
#endif

UBTD_TimeLimitRandom::UBTD_TimeLimitRandom(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "TimeLimit_Random";
	TimeLimit = 5.0f;

#if UE_VERSION_NEWER_THAN(5, 4, 0)
    INIT_DECORATOR_NODE_NOTIFY_FLAGS();
    bTickIntervals = true;

    // time limit always abort current branch
    bAllowAbortLowerPri = false;
    bAllowAbortNone = false;
    FlowAbortMode = EBTFlowAbortMode::Self;
#endif
}

FString UBTD_TimeLimitRandom::GetStaticDescription() const
{
	// basic info: result after time
	return FString::Printf(TEXT("TimeLimit: %s after %.1fs-%.1fs"),
		*UBehaviorTreeTypes::DescribeNodeResult(EBTNodeResult::Failed), TimeLimitRange.Min, TimeLimitRange.Max);
}

#if UE_VERSION_NEWER_THAN(5, 4, 0)
void UBTD_TimeLimitRandom::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
    Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

    const FBTAuxiliaryMemory* DecoratorMemory = GetSpecialNodeMemory<FBTAuxiliaryMemory>(NodeMemory);
    if (OwnerComp.GetWorld())
    {
        const float TimeLeft = DecoratorMemory->NextTickRemainingTime > 0.f ? DecoratorMemory->NextTickRemainingTime : 0.f;
        Values.Add(FString::Printf(TEXT("%s in %ss"),
            *UBehaviorTreeTypes::DescribeNodeResult(EBTNodeResult::Failed),
            *FString::SanitizeFloat(TimeLeft)));
    }
}

#if WITH_EDITOR
FName UBTD_TimeLimitRandom::GetNodeIconName() const
{
    return FName("BTEditor.Graph.BTNode.Decorator.TimeLimit.Icon");
}
#endif
#endif

void UBTD_TimeLimitRandom::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	TimeLimit = bUseIntegers
		? FMath::RandRange((int32)TimeLimitRange.Min, (int32)TimeLimitRange.Max)
		: FMath::RandRange(TimeLimitRange.Min, TimeLimitRange.Max);
	Super::OnNodeActivation(SearchData);
}

#if UE_VERSION_NEWER_THAN(5, 4, 0)
void UBTD_TimeLimitRandom::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    SetNextTickTime(NodeMemory, TimeLimit);
}

void UBTD_TimeLimitRandom::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnCeaseRelevant(OwnerComp, NodeMemory);

    reinterpret_cast<FBTimeLimitRandomMemory*>(NodeMemory)->bElapsed = false;
}

bool UBTD_TimeLimitRandom::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return !reinterpret_cast<FBTimeLimitRandomMemory*>(NodeMemory)->bElapsed;
}

void UBTD_TimeLimitRandom::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ensureMsgf(DeltaSeconds >= TimeLimit || FMath::IsNearlyEqual(DeltaSeconds, TimeLimit, UE_KINDA_SMALL_NUMBER),
        TEXT("Using SetNextTickTime in OnBecomeRelevant should guarantee that we are only getting ticked when the time limit is finished. DT=%f, TimeLimit=%f"),
        DeltaSeconds,
        TimeLimit);

    // Mark this decorator instance as Elapsed for calls to CalculateRawConditionValue
    reinterpret_cast<FBTimeLimitRandomMemory*>(NodeMemory)->bElapsed = true;

    // Set our next tick time to large value so we don't get ticked again in case the decorator
    // is still active after requesting execution (e.g. latent abort)
    SetNextTickTime(NodeMemory, FLT_MAX);

    OwnerComp.RequestExecution(this);
}

uint16 UBTD_TimeLimitRandom::GetInstanceMemorySize() const
{
    return sizeof(FBTimeLimitMemoryRandom);
}

void UBTD_TimeLimitRandom::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const EBTMemoryInit::Type InitType) const
{
    InitializeNodeMemory<FBTimeLimitMemoryRandom>(NodeMemory, InitType);
}

void UBTD_TimeLimitRandom::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const EBTMemoryClear::Type CleanupType) const
{
    CleanupNodeMemory<FBTimeLimitMemoryRandom>(NodeMemory, CleanupType);
}
#endif
