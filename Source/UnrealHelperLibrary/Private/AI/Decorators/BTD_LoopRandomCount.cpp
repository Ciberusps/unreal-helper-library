// Pavel Penkov 2025 All Rights Reserved.


#include "AI/Decorators/BTD_LoopRandomCount.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_LoopRandomCount)

UBTD_LoopRandomCount::UBTD_LoopRandomCount(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Loop_RandomCount";
	NumLoops = 3;
}

FString UBTD_LoopRandomCount::GetStaticDescription() const
{
	return FString::Printf(TEXT("Loops: %d-%d loops"), Range.Min, Range.Max);
}

void UBTD_LoopRandomCount::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	NumLoops = FMath::RandRange(Range.Min, Range.Max);
	Super::OnNodeActivation(SearchData);
}
