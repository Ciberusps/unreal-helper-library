// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_InAngle.h"

#include "AIController.h"
#include "Utils/UnrealHelperLibraryBPL.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_InAngle)

UBTD_InAngle::UBTD_InAngle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "InAngle";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

    // Accept only actors and vectors
    Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_InAngle, Target), AActor::StaticClass());
    Target.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_InAngle, Target));

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

bool UBTD_InAngle::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float CurrentAngle = GetCurrentAngle(OwnerComp, NodeMemory, bDrawDebug);

	return bDebugForceFalseCondition ? false : IsInAngle(CurrentAngle);
}

void UBTD_InAngle::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    UBlackboardData* BBAsset = GetBlackboardAsset();
    if (ensure(BBAsset))
    {
        Target.ResolveSelectedKey(*BBAsset);
    }
}

void UBTD_InAngle::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
    InitializeNodeMemory<FBTInAngleMemory>(NodeMemory, InitType);
}

void UBTD_InAngle::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
    CleanupNodeMemory<FBTInAngleMemory>(NodeMemory, CleanupType);
}

FString UBTD_InAngle::GetStaticDescription() const
{
    return Super::GetStaticDescription();
}

void UBTD_InAngle::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	Values.Add(FString::Printf(TEXT("CurrentAngle: %.2f"), GetCurrentAngle(OwnerComp, NodeMemory)));
}

#if WITH_EDITOR
FName UBTD_InAngle::GetNodeIconName() const
{
	return FName("GenericCommands.Redo");
}
#endif

float UBTD_InAngle::GetCurrentAngle(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, bool bDrawDebug_In) const
{
	float CurrentAngle = 0.0f;

	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return CurrentAngle;

    AAIController* OwnerController = OwnerComp.GetAIOwner();
    AActor* OwnerActor = OwnerController ? OwnerController->GetPawn() : nullptr;
    ACharacter* OwnerCharacter = IsValid(OwnerController) ? OwnerController->GetCharacter() : nullptr;
    AActor* TargetActor = nullptr;
    bool bTargetActorRequiredButNotSet = false;

    FVector TargetVector = FVector::ZeroVector;
    if (Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
    {
        UObject* BBValue = BlackboardComponent->GetValueAsObject(Target.SelectedKeyName);
        TargetActor = IsValid(BBValue) ? Cast<AActor>(BBValue) : nullptr;
        bTargetActorRequiredButNotSet = !IsValid(TargetActor);
        TargetVector = IsValid(TargetActor) ? TargetActor->GetActorLocation() : FVector::ZeroVector;
    }
    if (Target.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
    {
        TargetVector = BlackboardComponent->GetValueAsVector(Target.SelectedKeyName);
    }

	if (!IsValid(OwnerActor) || bTargetActorRequiredButNotSet) return CurrentAngle;

	CurrentAngle = UUnrealHelperLibraryBPL::RelativeAngleToVector(OwnerActor, TargetVector);

	if (bDrawDebug)
	{
		FVector LineStart = OwnerActor->GetActorLocation();
		FVector LineEnd = TargetVector;
		FVector TextLocation = (LineEnd - LineStart) / 2 + LineStart;
		bool bInAngle = IsInAngle(CurrentAngle);

        FAngleRange* InAngleRange = nullptr;

		if(IsValid(OwnerCharacter))
		{
		    float CapsuleSizeMultiplier = 6.0f;
			float AngleLineLength = OverrideDebugLinesDistance > 0.0f
		        ? OverrideDebugLinesDistance
		        : OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * CapsuleSizeMultiplier;
			for (FAngleRange IncludeRange : IncludeRanges)
			{
				bool bInThisAngle = UKismetMathLibrary::InRange_FloatFloat(CurrentAngle, IncludeRange.Range.GetLowerBoundValue(), IncludeRange.Range.GetUpperBoundValue(), true, true);
				float RangeAngle = IncludeRange.Range.GetLowerBoundValue() > 0.0f
					? IncludeRange.Range.GetUpperBoundValue() / 2
					: IncludeRange.Range.GetLowerBoundValue() / 2;

				// TODO bDebugUseSuccessColorFromRange
                // if (bInThisAngle)
                // {
                //     InAngleRange = &IncludeRange;
                // }

				FVector StartRangeLine = LineStart + OwnerCharacter->GetActorForwardVector().RotateAngleAxis(IncludeRange.Range.GetLowerBoundValue(), FVector(0, 0, 1)) * AngleLineLength;
				FVector EndRangeLine = LineStart + OwnerCharacter->GetActorForwardVector().RotateAngleAxis(IncludeRange.Range.GetUpperBoundValue(), FVector(0, 0, 1)) * AngleLineLength;
				// FVector RangeDirection = LineStart + CachedOwnerCharacter->GetActorForwardVector().RotateAngleAxis(RangeAngle, FVector(0, 0, 1)) * AngleLineLength * 0.3f;

				// DrawDebugCone(OwnerComp.GetWorld(), CachedOwnerCharacter->GetActorLocation(), RangeDirection, 500.0f, RangeAngle, 2.0f, 2, IncludeRange.DebugColor, false, -1, -1, 1.0f);
				DrawDebugLine(OwnerComp.GetWorld(), LineStart, StartRangeLine, bInThisAngle ? FColor::Green : IncludeRange.DebugColor, false, -1, -1, 4.0f);
				DrawDebugLine(OwnerComp.GetWorld(), LineStart, EndRangeLine, bInThisAngle ? FColor::Green : IncludeRange.DebugColor, false, -1, -1, 4.0f);
				// DrawDebugDirectionalArrow(OwnerComp.GetWorld(), LineStart, RangeDirection, 15.0f, bInThisAngle ? FColor::Green : IncludeRange.DebugColor, false, -1, -1, 3.0f);
			}
		}

	    FColor AngleTextColor = bInAngle ? FColor::Green : FColor::Red;
	    // if (InAngleRange != nullptr)
	    // {
	    //     AngleTextColor = InAngleRange->DebugColor;
	    // }

		DrawDebugLine(OwnerComp.GetWorld(), LineStart, LineEnd, bInAngle ? FColor::Green : FColor::Red, false, -1, -1, 2.0f);
		DrawDebugSphere(OwnerComp.GetWorld(), LineStart, 4.0f, 16, FColor::Blue, false, -1, -1, 2.0f);
		DrawDebugSphere(OwnerComp.GetWorld(), LineEnd, 4.0f, 16, FColor::Blue, false, -1, -1, 2.0f);
		DrawDebugString(OwnerComp.GetWorld(), TextLocation, FString::Printf(TEXT("Angle: %.2f"), CurrentAngle), nullptr,  AngleTextColor, 0, true);
	    DrawDebugString(OwnerComp.GetWorld(), OwnerActor->GetActorLocation(), FString::Printf(TEXT("ParentNode:\n%s \n\nNodeName:\n%s"), *GetParentNode()->NodeName, *GetMyNode()->NodeName), nullptr,  FColor::White, 0, true);
	}

	return CurrentAngle;
}

bool UBTD_InAngle::IsInAngle(float CurrentAngle) const
{
	bool bInThisAngle = false;
	for (FAngleRange IncludeRange : IncludeRanges)
	{
		bInThisAngle = UKismetMathLibrary::InRange_FloatFloat(CurrentAngle, IncludeRange.Range.GetLowerBoundValue(), IncludeRange.Range.GetUpperBoundValue(), true, true);
		if (bInThisAngle)
		{
			break;
		}
	}
	return bInThisAngle;
}
