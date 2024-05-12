// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_InRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"

UBTD_InRange::UBTD_InRange(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "InRange";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

uint16 UBTD_InRange::GetInstanceMemorySize() const
{
	return sizeof(FBTInRangeMemory);
}

FName UBTD_InRange::GetNodeIconName() const
{
	return FName("SoftwareCursor_UpDown");
}

float UBTD_InRange::GetCurrentDistance(const UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return 0.0f;

	AActor* SelfActor = OwnerComp.GetOwner();
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(Target.SelectedKeyName)); 
	if (!IsValid(SelfActor)) return 0.0f;
	
	float CurrentDistance = SelfActor->GetDistanceTo(TargetActor);

	if (bIncludeSelfCapsuleRadius)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(SelfActor);
		CurrentDistance -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	if (bIncludeTargetCapsuleRadius)
	{
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
		CurrentDistance -= TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
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
	bool bIsInRange = UKismetMathLibrary::InRange_FloatFloat(GetCurrentDistance(OwnerComp), Min, Max);

	return bIsInRange;
}

void UBTD_InRange::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	FBTInRangeMemory* DecoratorMemory = CastInstanceNodeMemory<FBTInRangeMemory>(NodeMemory);
	DecoratorMemory->CurrentDistance = 0.0f;
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
	Values.Add(FString::Printf(TEXT("CurrentDistance: %f"), GetCurrentDistance(OwnerComp)));
}
