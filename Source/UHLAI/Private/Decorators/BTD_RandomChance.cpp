// Pavel Penkov 2025 All Rights Reserved.


#include "Decorators/BTD_RandomChance.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "Engine/Engine.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UnrealType.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_RandomChance)

UBTD_RandomChance::UBTD_RandomChance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

	ScalableChanceLevelInBB.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_RandomChance, ScalableChanceLevelInBB));

#if WITH_EDITOR
	FProperty* StructProp = FindFProperty<FProperty>(
		GetClass(), GET_MEMBER_NAME_CHECKED(UBTD_RandomChance, ChanceChangePerStep));

	FStructProperty* StructProperty = CastField<FStructProperty>(StructProp);
	if (StructProperty)
	{
		FProperty* PotentiallyFloatProperty = StructProperty->Struct->FindPropertyByName(GET_MEMBER_NAME_CHECKED(FOperationOnBBValue, FloatValue));
		FStructProperty* FloatProperty = CastField<FStructProperty>(PotentiallyFloatProperty);
		if (FloatProperty)
		{
			FloatProperty->SetMetaData(TEXT("ClampMin"), TEXT("0.0"));
			FloatProperty->SetMetaData(TEXT("ClampMax"), TEXT("1.0"));
			FloatProperty->SetMetaData(TEXT("UIMin"), TEXT("0.0"));
			FloatProperty->SetMetaData(TEXT("UIMax"), TEXT("1.0"));	
		}
	}
#endif
}

bool UBTD_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float CurrentChance = GetCurrentChance(OwnerComp);
	bool bResult = UKismetMathLibrary::RandomBoolWithWeight(CurrentChance);

	float DrawDebugTime = DebugSettings.DrawDebugTime.GetValue(OwnerComp);
	FString ResultString = bResult ? "Success" : "Fail";
	FString ScalabilitySetting = "";
	if (ScaleType == EScalableChanceType::ScalableFloat)
	{
		UBlackboardData* BBAsset = GetBlackboardAsset();
		if (BBAsset->IsValid())
		{
			float ScaleLevel = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ScalableChanceLevelInBB.SelectedKeyName);
			ScalabilitySetting = FString::SanitizeFloat(ScaleLevel);
		}
	}
	else if (ScaleType == EScalableChanceType::BBDependantWithSteps)
	{
		UBlackboardData* BBAsset = GetBlackboardAsset();
		if (BBAsset->IsValid())
		{
			float ScaleLevel = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(StepsCount.SelectedKeyName);
			ScalabilitySetting = FString::SanitizeFloat(ScaleLevel);
		}
	}

	if (DebugSettings.bEnableDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1,
			DrawDebugTime, bResult ? FColor::Green : FColor::White,
			FString::Printf(TEXT("[UBTD_RandomChance] %s on \"%s\" with chance %.2f scaled by %s (%s)"), *ResultString, *GetParentNode()->GetName(), CurrentChance, *ScalabilitySetting, *UEnum::GetDisplayValueAsText(ScaleType).ToString())
		);	
	}
	
	return bResult;
}

FString UBTD_RandomChance::GetStaticDescription() const
{
	// TODO BB value preview?
	// float CurrentChance = bUseBlackboardValue ? GetBlackboardAsset()->GetKey(ChanceInBB.GetSelectedKeyID()) : Chance;
	return FString::Printf(TEXT("Chance - %.2f%% (%s)"), Chance * 100.0f, *Chance.ToString());
}

void UBTD_RandomChance::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	ChanceChangePerStep.SetCurrentBBKeyValueType(EBlackboardValueType::Float);
}

#if WITH_EDITOR
FName UBTD_RandomChance::GetNodeIconName() const
{
	// return FName("CodeView.ClassIcon");
	return FName("GraphEditor.StructGlyph");
}
#endif

float UBTD_RandomChance::GetCurrentChance(UBehaviorTreeComponent& OwnerComp) const
{
	float Result = 0.0f;
	switch (ScaleType)
	{
		case EScalableChanceType::None:
		{
			Result = Chance.GetValue(OwnerComp);
			break;
		}
		case EScalableChanceType::ScalableFloat:
		{
			float ScalableChanceLevel = 0.0f;
			UBlackboardData* BBAsset = GetBlackboardAsset();
			if (BBAsset->IsValid())
			{
				ScalableChanceLevel = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ScalableChanceLevelInBB.SelectedKeyName);
			}
			ScalableChanceLevel = ScalableChanceLevelInBB.IsNone() ? 0.0f : ScalableChanceLevel;
			Result = ScalableChance.GetValueAtLevel(ScalableChanceLevel);
			break;
		}
		case EScalableChanceType::BBDependantWithSteps:
		{
			Result = Chance.GetValue(OwnerComp);
			float StepsCountValue = 0.0f;
			UBlackboardData* BBAsset = GetBlackboardAsset();
			if (BBAsset->IsValid())
			{
				StepsCountValue = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(StepsCount.SelectedKeyName);
			}
			// for every step calculate float value
			int32 StepsCountInt = FMath::RoundToInt(StepsCountValue);
			for (int32 i = 0; i < StepsCountInt; i++)
			{
				Result = ChanceChangePerStep.CalculateFloatResult(OwnerComp, Result);
			}
			break;
		}
		default:
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