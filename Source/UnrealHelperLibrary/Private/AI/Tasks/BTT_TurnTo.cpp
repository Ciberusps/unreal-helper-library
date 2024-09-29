// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_TurnTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_TurnTo)

void UBTT_TurnTo::SetupPreset_Default_90_180()
{
    TurnSettings.SetupPreset_Default_90_180();
}

void UBTT_TurnTo::SetupPreset_BigEnemy_90_180()
{
    TurnSettings.SetupPreset_BigEnemy_90_180();
}

void UBTT_TurnTo::SetupPreset_45_90_180()
{
    TurnSettings.SetupPreset_45_90_180();
}

void UBTT_TurnTo::SetupPreset_15_45_90_180()
{
    TurnSettings.SetupPreset_15_45_90_180();
}

void UBTT_TurnTo::SetupPreset_15_30_45_90_180()
{
    TurnSettings.SetupPreset_15_30_45_90_180();
}

void UBTT_TurnTo::Cleanup()
{
    TurnSettings.Cleanup();
}

UBTT_TurnTo::UBTT_TurnTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Precision(10.f)
{
	NodeName = "TurnTo";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	// accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_TurnTo, BlackboardKey), AActor::StaticClass());
    // TODO add support for Vectors/Rotators
	// BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_TurnTo, BlackboardKey));
	// BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_TurnTo, BlackboardKey));
}

void UBTT_TurnTo::PostInitProperties()
{
	Super::PostInitProperties();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

void UBTT_TurnTo::PostLoad()
{
	Super::PostLoad();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

namespace
{
	FORCEINLINE_DEBUGGABLE FVector::FReal CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
            ? 1.f
            : VectorA.CosineAngle2D(VectorB);
	}
}

EBTNodeResult::Type UBTT_TurnTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FBTTurnToMemory* MyMemory = CastInstanceNodeMemory<FBTTurnToMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->Reset();

	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();

	const FVector PawnLocation = Pawn->GetActorLocation();
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);

		if (ActorValue != NULL)
		{
			const FVector::FReal AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector()
				, (ActorValue->GetActorLocation() - PawnLocation));

			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				AIController->SetFocus(ActorValue, EAIFocusPriority::Gameplay);
				MyMemory->FocusActorSet = ActorValue;
				MyMemory->bActorSet = true;
			    if (Pawn->GetClass()->ImplementsInterface(UUHLActorSettings::StaticClass()))
			    {
			        MyMemory->TurnSettings = GetTurnSettings(Pawn, MyMemory->bCurrentTurnSettingsSet);
			    }
				Result = EBTNodeResult::InProgress;
			}
		}
	}
    // TODO add support for Vectors/Rotators
	// else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	// {
	// 	const FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	//
	// 	if (FAISystem::IsValidLocation(KeyValue))
	// 	{
	// 		const FVector::FReal AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector()
	// 			, (KeyValue - PawnLocation));
	//
	// 		if (AngleDifference >= PrecisionDot)
	// 		{
	// 			Result = EBTNodeResult::Succeeded;
	// 		}
	// 		else
	// 		{
	// 			AIController->SetFocalPoint(KeyValue, EAIFocusPriority::Gameplay);
	// 			MyMemory->FocusLocationSet = KeyValue;
	// 			Result = EBTNodeResult::InProgress;
	// 		}
	// 	}
	// }
	// else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	// {
	// 	const FRotator KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());
	//
	// 	if (FAISystem::IsValidRotation(KeyValue))
	// 	{
	// 		const FVector DirectionVector = KeyValue.Vector();
	// 		const FVector::FReal AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(), DirectionVector);
	//
	// 		if (AngleDifference >= PrecisionDot)
	// 		{
	// 			Result = EBTNodeResult::Succeeded;
	// 		}
	// 		else
	// 		{
	// 			const FVector FocalPoint = PawnLocation + DirectionVector * 10000.0f;
	// 			// set focal somewhere far in the indicated direction
	// 			AIController->SetFocalPoint(FocalPoint, EAIFocusPriority::Gameplay);
	// 			MyMemory->FocusLocationSet = FocalPoint;
	// 			Result = EBTNodeResult::InProgress;
	// 		}
	// 	}
	// }

	return Result;
}

void UBTT_TurnTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

    FBTTurnToMemory* MyMemory = CastInstanceNodeMemory<FBTTurnToMemory>(NodeMemory);
    check(MyMemory);

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		const FVector PawnDirection = AIController->GetPawn()->GetActorForwardVector();
		const FVector FocalPoint = AIController->GetFocalPointForPriority(EAIFocusPriority::Gameplay);
	    ACharacter* AICharacter = AIController->GetCharacter();

		if (FocalPoint != FAISystem::InvalidLocation)
		{
		    float DeltaAngleRad = CalculateAngleDifferenceDot(PawnDirection, FocalPoint - AIController->GetPawn()->GetActorLocation());
		    // float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(DeltaAngleRad));
		    float DeltaAngle = UUnrealHelperLibraryBPL::RelativeAngleToActor(AICharacter, MyMemory->FocusActorSet);
		    UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("DeltaAngle %f"), DeltaAngle), "", "", "", "", "", "", "", "", "", -1, FName("Test"));

			if (DeltaAngleRad >= PrecisionDot)
			{
			    UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("TurnRange->bOverrideStopMontageOnGoalReached %hhd"), MyMemory->CurrentTurnRange.bOverrideStopMontageOnGoalReached));
			    bool bCanStopMontage = false;
			    if (MyMemory->CurrentTurnRange.bOverrideStopMontageOnGoalReached)
			    {
			        bCanStopMontage = MyMemory->CurrentTurnRange.bStopMontageOnGoalReached;
			    }
			    else
			    {
			        bCanStopMontage = MyMemory->TurnSettings.bStopMontageOnGoalReached;
			    }

			    if (MyMemory && bCanStopMontage)
			    {
			        AICharacter->StopAnimMontage();
			        CleanUp(*AIController, NodeMemory);
			        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			    }
			    else
			    {
			        CleanUp(*AIController, NodeMemory);
			        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			    }
			}
		    else
		    {
		        if (IsTurnWithAnimationRequired(AICharacter))
		        {
		            MyMemory->CurrentTurnRange = GetTurnRange(DeltaAngle, MyMemory->bCurrentTurnRangeSet, MyMemory->TurnSettings);
		            if (MyMemory->bCurrentTurnRangeSet && MyMemory->CurrentTurnRange.AnimMontage)
		            {
		                AICharacter->PlayAnimMontage(MyMemory->CurrentTurnRange.AnimMontage);
		            }

		            // TODO тут ошибка?
		            // finish if no turn animation found and "bTurnOnlyWithAnims"
		            if (!MyMemory->bCurrentTurnRangeSet && MyMemory->TurnSettings.bTurnOnlyWithAnims)
		            {
		                CleanUp(*AIController, NodeMemory);
		                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		            }
		        }
		    }
		}
		else
		{
			CleanUp(*AIController, NodeMemory);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}

void UBTT_TurnTo::CleanUp(AAIController& AIController, uint8* NodeMemory)
{
	FBTTurnToMemory* MyMemory = (FBTTurnToMemory*)NodeMemory;
	check(MyMemory);

	bool bClearFocus = false;
	if (MyMemory->bActorSet)
	{
		bClearFocus = (MyMemory->FocusActorSet == AIController.GetFocusActorForPriority(EAIFocusPriority::Gameplay));
	}
	else
	{
		bClearFocus = (MyMemory->FocusLocationSet == AIController.GetFocalPointForPriority(EAIFocusPriority::Gameplay));
	}

	if (bClearFocus)
	{
		AIController.ClearFocus(EAIFocusPriority::Gameplay);
	}
}

bool UBTT_TurnTo::IsTurnWithAnimationRequired(ACharacter* Character) const
{
    if (!Character) return false;
    if (Character->IsPlayingRootMotion()) return false;
    return true;
}

FTurnRange UBTT_TurnTo::GetTurnRange(float DeltaAngle, bool& bCurrentTurnRangeSet, FTurnSettings TurnSettings_In)
{
    FTurnRange Result;
    bCurrentTurnRangeSet = false;
    for (TTuple<FString, FTurnRanges> TurnToRange : TurnSettings_In.TurnRangesGroups)
    {
        for (FTurnRange Range : TurnToRange.Value.TurnRanges)
        {
            if (Range.Range.Contains(DeltaAngle))
            {
                Result = Range;
                bCurrentTurnRangeSet = true;
                break;
            }
        }
        if (bCurrentTurnRangeSet)
        {
            break;
        }
    }
    return Result;
}

FTurnSettings UBTT_TurnTo::GetTurnSettings(AActor* Actor, bool& bCurrentTurnSettingsSet)
{
    FTurnSettings Result;
    bCurrentTurnSettingsSet = false;
    if (SettingsSource == EUHLSettingsSource::Actor)
    {
        Result = IUHLActorSettings::Execute_GetTurnSettings(Actor);
        bCurrentTurnSettingsSet = true;
    }
    if (SettingsSource == EUHLSettingsSource::DataAsset)
    {
        Result = RotateToAnimationsDataAsset->TurnSettings;
        bCurrentTurnSettingsSet = true;
    }
    if (SettingsSource == EUHLSettingsSource::Node)
    {
        Result = TurnSettings;
        bCurrentTurnSettingsSet = true;
    }
    return Result;
}

EBTNodeResult::Type UBTT_TurnTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController != NULL)
	{
		CleanUp(*AIController, NodeMemory);
	}

	return EBTNodeResult::Aborted;
}

void UBTT_TurnTo::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	FString KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	Values.Add(FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc));

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController != NULL && AIController->GetPawn() != NULL)
	{
		const FVector PawnDirection = AIController->GetPawn()->GetActorForwardVector();
		const FVector FocalPoint = AIController->GetFocalPointForPriority(EAIFocusPriority::Gameplay);

		if (FocalPoint != FAISystem::InvalidLocation)
		{
			const FVector::FReal CurrentAngleRadians = CalculateAngleDifferenceDot(PawnDirection, (FocalPoint - AIController->GetPawn()->GetActorLocation()));
			Values.Add(FString::Printf(TEXT("Current angle: %.2f"), FMath::RadiansToDegrees(FMath::Acos(CurrentAngleRadians))));
		}
		else
		{
			Values.Add(TEXT("FocalPoint is an Invalid Location"));
		}
	}
	else
	{
		Values.Add(TEXT("Controller or Pawn is NULL"));
	}
}

FString UBTT_TurnTo::GetStaticDescription() const
{
	FString KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}

void UBTT_TurnTo::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTTurnToMemory>(NodeMemory, InitType);
}

void UBTT_TurnTo::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTTurnToMemory>(NodeMemory, CleanupType);
}
