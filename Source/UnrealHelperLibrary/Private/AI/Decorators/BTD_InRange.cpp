// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_InRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"

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

uint16 UBTD_InRange::GetInstanceMemorySize() const
{
	return sizeof(FBTInRangeMemory);
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

	AActor* SelfActor = OwnerComp.GetOwner();
	AActor* TargetActor = nullptr;
    bool bTargetActorRequiredButNotSet = false;
    if (Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
    {
        UObject* BBValue = BlackboardComponent->GetValueAsObject(Target.SelectedKeyName);
        TargetActor = IsValid(BBValue) ? Cast<AActor>(BBValue) : nullptr;
        bTargetActorRequiredButNotSet = IsValid(TargetActor) ? false : true;
    }
    FVector TargetVector = Target.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()
        ? BlackboardComponent->GetValueAsVector(Target.SelectedKeyName)
        : IsValid(TargetActor) ? TargetActor->GetActorLocation() : FVector::Zero();
	if (!IsValid(SelfActor) || bTargetActorRequiredButNotSet) return CurrentDistance;

    AController* OwnerCharacterController = Cast<AController>(SelfActor);
    ACharacter* OwnerCharacter = IsValid(OwnerCharacterController) ? OwnerCharacterController->GetCharacter() : nullptr;
    CurrentDistance = FVector::Distance(SelfActor->GetActorLocation(), TargetVector);

	if (bIncludeSelfCapsuleRadius && IsValid(OwnerCharacter))
	{
		CurrentDistance -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	// TODO cache TargetCharacter when possible (on BBValue change?)
    ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (IsValid(TargetActor) && bIncludeTargetCapsuleRadius)
	{
		CurrentDistance -= TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	if (bDrawDebug_In)
	{
		FVector LineStart = OwnerCharacter->GetActorLocation();
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
		DrawDebugString(OwnerComp.GetWorld(), TextLocation, FString::Printf(TEXT("Distance: %.2f"), CurrentDistance), nullptr, bInRange ? FColor::Green : FColor::Red, DebugLifetime < 0 ? 0 : DebugLifetime, true);
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
	FBTInRangeMemory* DecoratorMemory = CastInstanceNodeMemory<FBTInRangeMemory>(NodeMemory);
}

void UBTD_InRange::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryClear::Type CleanupType) const
{
	// if (CleanupType == EBTMemoryClear::Destroy)
	// {
	Super::CleanupMemory(OwnerComp, NodeMemory, CleanupType);
	// }
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
