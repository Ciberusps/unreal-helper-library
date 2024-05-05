// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_RandomChance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

UBTD_RandomChance::UBTD_RandomChance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

	ChanceInBB.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_RandomChance, ChanceInBB));
}

bool UBTD_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float CurrentChance = 0.0f;
	if (bUseBlackboardValue)
	{
		CurrentChance = FMath::Clamp(OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ChanceInBB.SelectedKeyName), 0.0f, 1.0f);
	}
	// else if (bUseUnclamped)
	// {
	// 	CurrentChance = ChanceUnclamped
	// }
	else
	{
		CurrentChance = Chance;
	}

	return UKismetMathLibrary::RandomBoolWithWeight(CurrentChance);
}

FString UBTD_RandomChance::GetStaticDescription() const
{
	// TODO BB value preview?	
	// float CurrentChance = bUseBlackboardValue ? GetBlackboardAsset()->GetKey(ChanceInBB.GetSelectedKeyID()) : Chance; 
	return FString::Printf(TEXT("Chance - %.2f%% (%.2f)"), Chance * 100.0f, Chance);
}

FName UBTD_RandomChance::GetNodeIconName() const
{
	// return FName("CodeView.ClassIcon");
	return FName("GraphEditor.StructGlyph");
}
