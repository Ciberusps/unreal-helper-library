// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_LoopRandomCount.h"

UBTDecorator_LoopRandomCount::UBTDecorator_LoopRandomCount(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Loop_RandomCount";
	NumLoops = 3;
}

FString UBTDecorator_LoopRandomCount::GetStaticDescription() const
{
	return FString::Printf(TEXT("Loops: %d-%d loops"), Range.Min, Range.Max);
}

void UBTDecorator_LoopRandomCount::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	NumLoops = FMath::RandRange(Range.Min, Range.Max);
	Super::OnNodeActivation(SearchData);
}
