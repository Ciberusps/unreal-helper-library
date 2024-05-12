// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DebugPrintBBValue.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UBTT_DebugPrintBBValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DebugPrintBBValue(const FObjectInitializer& ObjectInitializer);
	
	// UPROPERTY(Category=Decorator, EditAnywhere)
	// FString Line1;
	UPROPERTY(Category=Decorator, EditAnywhere)
	FBlackboardKeySelector BlackboardKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FString GetBBKeyDescription(UBehaviorTreeComponent& OwnerComp);
	virtual FString GetStaticDescription() const override;
};
