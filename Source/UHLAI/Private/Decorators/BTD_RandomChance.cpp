// Pavel Penkov 2025 All Rights Reserved.


#include "Decorators/BTD_RandomChance.h"

#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_RandomChance)

UBTD_RandomChance::UBTD_RandomChance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

    // TODO value from blackboard
	// ChanceInBB.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_RandomChance, ChanceInBB));
}

bool UBTD_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float CurrentChance = 0.0f;
    // TODO value from blackboard
	// if (bUseBlackboardValue)
	// {
	// 	CurrentChance = FMath::Clamp(OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ChanceInBB.SelectedKeyName), 0.0f, 1.0f);
	// }
	// else if (bUseUnclamped)
	// {
	// 	CurrentChance = ChanceUnclamped
	// }
	// else
	// {
		CurrentChance = Chance.GetValue(OwnerComp);
	// }

	return UKismetMathLibrary::RandomBoolWithWeight(CurrentChance);
}

FString UBTD_RandomChance::GetStaticDescription() const
{
	// TODO BB value preview?
	// float CurrentChance = bUseBlackboardValue ? GetBlackboardAsset()->GetKey(ChanceInBB.GetSelectedKeyID()) : Chance;
	return FString::Printf(TEXT("Chance - %.2f%% (%s)"), Chance * 100.0f, *Chance.ToString());
}

#if WITH_EDITOR
FName UBTD_RandomChance::GetNodeIconName() const
{
	// return FName("CodeView.ClassIcon");
	return FName("GraphEditor.StructGlyph");
}
#endif

float UBTD_RandomChance::GetCurrentChance(UBehaviorTreeComponent& OwnerComp)
{
	float Result = 0.0f;
	switch (ScaleType)
	{
		case EScalableChanceType::None:
			Result = Chance.GetValue(OwnerComp);
			break;
		
		case EScalableChanceType::ScalableFloat:
			float ScalableChanceLevel = 0.0f;
			UBlackboardData* BBAsset = GetBlackboardAsset();
			if (ensure(BBAsset))
			{
				ScalableChanceLevel = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ScalableChanceValueLevelFromBB.SelectedKeyName);
			}
			ScalableChanceLevel = ScalableChanceValueLevelFromBB.IsNone() ? 0.0f : ScalableChanceLevel;
			Result = ScalableChanceValue.GetValueAtLevel(ScalableChanceLevel);
			break;

		case EScalableChanceType::BBDependantWithSteps:
			
			break;
	}

	// clamp if MaxChance defined it could be 0.6f or something like that
	if (ScaleType != EScalableChanceType::None)
	{
		Result = FMath::Clamp(Result, 0.0f, MaxChance.GetValue(OwnerComp));
	}

	// clamp anything chance range 0.0f-1.0f
	Result = FMath::Clamp(Result, 0.0f, 1.0f);
	
	return Result;
}