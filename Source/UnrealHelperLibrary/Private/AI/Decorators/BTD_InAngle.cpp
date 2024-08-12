// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_InAngle.h"

#include "KismetAnimationLibrary.h"
#include "Utils/UnrealHelperLibraryBPL.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

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
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
	FBTInAngleMemory* DecoratorMemory = CastInstanceNodeMemory<FBTInAngleMemory>(NodeMemory);
	DecoratorMemory->OwnerCharacter = nullptr;
	DecoratorMemory->TargetCharacter = nullptr;
}

uint16 UBTD_InAngle::GetInstanceMemorySize() const
{
	return sizeof(FBTInAngleMemory);
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

EBlackboardNotificationResult UBTD_InAngle::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard,
                                                                       FBlackboard::FKey ChangedKeyID)
{
	if (ChangedKeyID == Target.GetSelectedKeyID())
	{
		UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());

		const int32 NodeInstanceIdx = BehaviorComp->FindInstanceContainingNode(this);
		FBTInAngleMemory* Memory = CastInstanceNodeMemory<FBTInAngleMemory>(BehaviorComp->GetNodeMemory(this, NodeInstanceIdx));

		TryCacheTargetCharacter(&Blackboard, Memory);
	}

	return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
}

float UBTD_InAngle::GetCurrentAngle(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, bool bDrawDebug_In) const
{
	float CurrentAngle = 0.0f;

	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return CurrentAngle;

	FBTInAngleMemory* Memory = CastInstanceNodeMemory<FBTInAngleMemory>(NodeMemory);
	if (Memory == nullptr) return CurrentAngle;

	AActor* SelfActor = OwnerComp.GetOwner();
	ACharacter* OwnerCharacter = Memory->OwnerCharacter.Get();
	if (!IsValid(OwnerCharacter))
	{
		AController* OwnerCharacterController = Cast<AController>(SelfActor);
		OwnerCharacter = IsValid(OwnerCharacterController) ? OwnerCharacterController->GetCharacter() : nullptr;
		Memory->OwnerCharacter = OwnerCharacter;
	}

	ACharacter* TargetCharacter = Memory->TargetCharacter.Get();
	if (!IsValid(TargetCharacter))
	{
		TryCacheTargetCharacter(BlackboardComponent, Memory);
		// TargetCharacter = Cast<ACharacter>(TargetActor);
		// Memory->TargetCharacter = TargetCharacter;
	}

	if (!IsValid(SelfActor) || !IsValid(Memory->TargetCharacter.Get())) return CurrentAngle;

	CurrentAngle = UUnrealHelperLibraryBPL::RelativeAngleToActor(SelfActor, Memory->TargetCharacter.Get());

	if (bDrawDebug)
	{
		FVector LineStart = SelfActor->GetActorLocation();
		FVector LineEnd = Memory->TargetCharacter->GetActorLocation();
		FVector TextLocation = (LineEnd - LineStart) / 2 + LineStart;
		bool bInAngle = IsInAngle(CurrentAngle);

		if(IsValid(OwnerCharacter))
		{
			float AngleLineLength = OverrideDebugLinesDistance > 0.0f
		        ? OverrideDebugLinesDistance
		        : OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * 6.0f;
			// CurrentDistance -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
			for (FAngleRange IncludeRange : IncludeRanges)
			{
				bool bInThisAngle = UKismetMathLibrary::InRange_FloatFloat(CurrentAngle, IncludeRange.Range.GetLowerBoundValue(), IncludeRange.Range.GetUpperBoundValue(), true, true);
				float RangeAngle = IncludeRange.Range.GetLowerBoundValue() > 0.0f
					? IncludeRange.Range.GetUpperBoundValue() / 2
					: IncludeRange.Range.GetLowerBoundValue() / 2;

				FVector StartRangeLine = LineStart + OwnerCharacter->GetActorForwardVector().RotateAngleAxis(IncludeRange.Range.GetLowerBoundValue(), FVector(0, 0, 1)) * AngleLineLength;
				FVector EndRangeLine = LineStart + OwnerCharacter->GetActorForwardVector().RotateAngleAxis(IncludeRange.Range.GetUpperBoundValue(), FVector(0, 0, 1)) * AngleLineLength;
				// FVector RangeDirection = LineStart + CachedOwnerCharacter->GetActorForwardVector().RotateAngleAxis(RangeAngle, FVector(0, 0, 1)) * AngleLineLength * 0.3f;

				// DrawDebugCone(OwnerComp.GetWorld(), CachedOwnerCharacter->GetActorLocation(), RangeDirection, 500.0f, RangeAngle, 2.0f, 2, IncludeRange.DebugColor, false, -1, -1, 1.0f);
				DrawDebugLine(OwnerComp.GetWorld(), LineStart, StartRangeLine, bInThisAngle ? FColor::Green : IncludeRange.DebugColor, false, -1, -1, 4.0f);
				DrawDebugLine(OwnerComp.GetWorld(), LineStart, EndRangeLine, bInThisAngle ? FColor::Green : IncludeRange.DebugColor, false, -1, -1, 4.0f);
				// DrawDebugDirectionalArrow(OwnerComp.GetWorld(), LineStart, RangeDirection, 15.0f, bInThisAngle ? FColor::Green : IncludeRange.DebugColor, false, -1, -1, 3.0f);
			}
		}

		DrawDebugLine(OwnerComp.GetWorld(), LineStart, LineEnd, bInAngle ? FColor::Green : FColor::Red, false, -1, -1, 2.0f);
		DrawDebugSphere(OwnerComp.GetWorld(), LineStart, 4.0f, 16, FColor::Blue, false, -1, -1, 2.0f);
		DrawDebugSphere(OwnerComp.GetWorld(), LineEnd, 4.0f, 16, FColor::Blue, false, -1, -1, 2.0f);
		DrawDebugString(OwnerComp.GetWorld(), TextLocation, FString::Printf(TEXT("Angle: %.2f"), CurrentAngle), nullptr,  bInAngle ? FColor::Green : FColor::White, 0, true);
	    DrawDebugString(OwnerComp.GetWorld(), SelfActor->GetActorLocation(), FString::Printf(TEXT("ParentNode:\n%s \n\nNodeName:\n%s"), *GetParentNode()->NodeName, *GetMyNode()->NodeName), nullptr,  FColor::White, 0, true);
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

void UBTD_InAngle::TryCacheTargetCharacter(const UBlackboardComponent* BlackboardComponent, FBTInAngleMemory* NodeMemory) const
{
	// const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	// Target.ResolveSelectedKey(*BlackboardAsset);

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(Target.SelectedKeyName));
	if (!IsValid(TargetActor)) return;

	// if (Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	// {
		// UObject* KeyValue = Blackboard->GetValue<UBlackboardKeyType_Object>(Target.GetSelectedKeyID());
		// AActor* TargetActor = Cast<AActor>(KeyValue);
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
		if (TargetCharacter)
		{
			NodeMemory->TargetCharacter = TargetCharacter;
		}
	// }
}

