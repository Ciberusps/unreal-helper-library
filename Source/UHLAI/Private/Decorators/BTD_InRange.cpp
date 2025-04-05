// Pavel Penkov 2025 All Rights Reserved.


#include "Decorators/BTD_InRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_InRange)

UBTD_InRange::UBTD_InRange(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "InRange";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

    // Accept only actors and vectors
    Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_InRange, Target), AActor::StaticClass());
    Target.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_InRange, Target));

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

#if WITH_EDITOR
FName UBTD_InRange::GetNodeIconName() const
{
	return FName("SoftwareCursor_UpDown");
}
#endif

float UBTD_InRange::GetCurrentDistance(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, bool bDrawDebug_In) const
{
	float CurrentDistance = 0.0f;

	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return CurrentDistance;

    AAIController* OwnerController = OwnerComp.GetAIOwner();
    AActor* OwnerActor = OwnerController ? OwnerController->GetPawn() : nullptr;
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

    if (!IsValid(OwnerActor) || bTargetActorRequiredButNotSet) return CurrentDistance;

    ACharacter* OwnerCharacter = IsValid(OwnerController) ? OwnerController->GetCharacter() : nullptr;
    CurrentDistance = bCalculateDistanceIn2D
        ? FVector::Dist2D(OwnerActor->GetActorLocation(), TargetVector)
        : FVector::Distance(OwnerActor->GetActorLocation(), TargetVector);

	if (bIncludeSelfCapsuleRadius && IsValid(OwnerCharacter))
	{
		CurrentDistance -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	// TODO cache TargetCharacter when possible (on BBValue change?)
    ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (IsValid(TargetCharacter) && bIncludeTargetCapsuleRadius)
	{
		CurrentDistance -= TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	if (bDrawDebug_In)
	{
		FVector LineStart = OwnerActor->GetActorLocation();
		FVector LineEnd = TargetVector;
		FVector TextLocation = (LineEnd - LineStart) / 2 + LineStart;
		bool bInRange = UKismetMathLibrary::InRange_FloatFloat(CurrentDistance, Min, Max);

		if (bIncludeSelfCapsuleRadius && IsValid(OwnerCharacter))
		{
			UCapsuleComponent* OwnerCharacterCapsule = OwnerCharacter->GetCapsuleComponent();
			DrawDebugCapsule(OwnerComp.GetWorld(), OwnerCharacterCapsule->GetComponentLocation(),
				OwnerCharacterCapsule->GetScaledCapsuleHalfHeight(),
				OwnerCharacterCapsule->GetScaledCapsuleRadius(),
				OwnerCharacterCapsule->GetComponentRotation().Quaternion(), FColor::Blue,
				false,DebugLifetime, 0, 2.0f);
			LineStart = (LineEnd - LineStart).GetSafeNormal() * OwnerCharacterCapsule->GetScaledCapsuleRadius() + OwnerCharacter->GetActorLocation();
		}

		if (bIncludeTargetCapsuleRadius && IsValid(TargetCharacter))
		{
			UCapsuleComponent* TargetCharacterCapsule = TargetCharacter->GetCapsuleComponent();
			DrawDebugCapsule(OwnerComp.GetWorld(), TargetCharacterCapsule->GetComponentLocation(),
				TargetCharacterCapsule->GetScaledCapsuleHalfHeight(),
				TargetCharacterCapsule->GetScaledCapsuleRadius(),
				TargetCharacterCapsule->GetComponentRotation().Quaternion(), FColor::Blue,
				false,DebugLifetime, 0, 2.0f);
			LineEnd = (LineStart - LineEnd).GetSafeNormal() * TargetCharacterCapsule->GetScaledCapsuleRadius() + TargetVector;
		}

		DrawDebugLine(OwnerComp.GetWorld(), LineStart, LineEnd, bInRange ? FColor::Green : FColor::Red, false, DebugLifetime, -1, 2.0f);
		DrawDebugSphere(OwnerComp.GetWorld(), LineStart, 5.0f, 16, FColor::Blue, false, DebugLifetime, DebugLifetime, 2.0f);
		DrawDebugSphere(OwnerComp.GetWorld(), LineEnd, 5.0f, 16, FColor::Blue, false, DebugLifetime, DebugLifetime, 2.0f);
		DrawDebugString(OwnerComp.GetWorld(), TextLocation, FString::Printf(TEXT("Distance%s: %.2f"), bCalculateDistanceIn2D ? TEXT("(2D)") : TEXT("(3D)"), CurrentDistance), nullptr, bInRange ? FColor::Green : FColor::Red, DebugLifetime < 0 ? 0 : DebugLifetime, true);
	    DrawDebugString(OwnerComp.GetWorld(), OwnerCharacter->GetActorLocation(), FString::Printf(TEXT("ParentNode:\n%s \n\nNodeName:\n%s"), *GetParentNode()->NodeName, *GetMyNode()->NodeName), nullptr,  FColor::White, DebugLifetime < 0 ? 0 : DebugLifetime, true);
	}

	return CurrentDistance;
}

void UBTD_InRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	RequestAbort(OwnerComp, EvaluateAbortType(OwnerComp));
}

bool UBTD_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    float CurrentDistance = GetCurrentDistance(OwnerComp, NodeMemory, bDrawDebug);
	bool bIsInRange = UKismetMathLibrary::InRange_FloatFloat(CurrentDistance, Min, Max);
	return bIsInRange;
}

void UBTD_InRange::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    UBlackboardData* BBAsset = GetBlackboardAsset();
    if (ensure(BBAsset))
    {
        Target.ResolveSelectedKey(*BBAsset);
    }
}

void UBTD_InRange::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
    InitializeNodeMemory<FBTInRangeMemory>(NodeMemory, InitType);
}

void UBTD_InRange::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryClear::Type CleanupType) const
{
    CleanupNodeMemory<FBTInRangeMemory>(NodeMemory, CleanupType);
}

FString UBTD_InRange::GetStaticDescription() const
{
	FString NodeDescription = GetPropertiesDetails();
	// NodeDescription += FString::Printf(TEXT("Target: %s\n"), *Target.SelectedKeyName.ToString());
	// NodeDescription += FString::Printf(TEXT("Min: %2f\n"), Min);
	// NodeDescription += FString::Printf(TEXT("Max: %2f\n"), Max);
	// NodeDescription += FString::Printf(TEXT("bIncludeSelfCapsuleRadius: %hs\n"), BTOA(bIncludeSelfCapsuleRadius));
	// NodeDescription += FString::Printf(TEXT("bIncludeTargetCapsuleRadius: %hs\n"), BTOA(bIncludeTargetCapsuleRadius));
	return FString::Printf(TEXT("%s%s"), *Super::GetStaticDescription(), *NodeDescription);
}

void UBTD_InRange::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

    // no need to cache value its only works in editor
	Values.Add(FString::Printf(TEXT("CurrentDistance: %.2f"), GetCurrentDistance(OwnerComp, NodeMemory, false)));
}
