// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FireGameplayEvent.generated.h"

struct FBlackboardKeySelector;

/**
 * 
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTT_FireGameplayEvent : public UBTTaskNode
{
	GENERATED_BODY()

	
public:
	UBTT_FireGameplayEvent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FGameplayTag EventTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Instigator;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector OptionalObject;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector OptionalObject2;

	// tags that required on instigator to fire event ??
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FGameplayTagContainer InstigatorTags = {};

	// tags that required on target to fire event ??
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FGameplayTagContainer TargetTags = {};
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	float EventMagnitude = 0.0f;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
