// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_TimeLimitRandom.h"


UBTDecorator_TimeLimitRandom::UBTDecorator_TimeLimitRandom(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "TimeLimit_Random";
	TimeLimit = 5.0f;
}

FString UBTDecorator_TimeLimitRandom::GetStaticDescription() const
{
	// basic info: result after time
	return FString::Printf(TEXT("TimeLimit: %s after %.1fs-%.1fs"),
		*UBehaviorTreeTypes::DescribeNodeResult(EBTNodeResult::Failed), TimeLimitRange.Min, TimeLimitRange.Max);
}

void UBTDecorator_TimeLimitRandom::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	TimeLimit = bUseIntegers
		? FMath::RandRange((int32)TimeLimitRange.Min, (int32)TimeLimitRange.Max)
		: FMath::RandRange(TimeLimitRange.Min, TimeLimitRange.Max);
	Super::OnNodeActivation(SearchData);
}
