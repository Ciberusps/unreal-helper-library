// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_Base.h"

#include "BlueprintNodeHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_Base::UBTD_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsObservingBB = false;
}

void UBTD_Base::InitializeProperties()
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		UClass* StopAtClass = UBTD_Base::StaticClass();
		BlueprintNodeHelpers::CollectPropertyData(this, StopAtClass, PropertyData);

		bIsObservingBB = BlueprintNodeHelpers::HasAnyBlackboardSelectors(this, StopAtClass);
	}
	else
	{
		// Make sure if the users started to observe a BB key after we instantiated this node, that this value get propagated correctly
		bIsObservingBB = GetClass()->GetDefaultObject<UBTD_Base>()->bIsObservingBB;
	}
}

void UBTD_Base::PostInitProperties()
{
	Super::PostInitProperties();

	InitializeProperties();
}

FString UBTD_Base::GetPropertiesDetails() const
{
	FString ReturnDesc = "";
	UBTD_Base* CDO = (UBTD_Base*)(GetClass()->GetDefaultObject());
	if (bShowPropertyDetails && CDO)
	{
		UClass* StopAtClass = UBTD_Base::StaticClass();
		FString PropertyDesc = BlueprintNodeHelpers::CollectPropertyDescription(this, StopAtClass, CDO->PropertyData);
		if (PropertyDesc.Len())
		{
			ReturnDesc += TEXT(":\n\n");
			ReturnDesc += PropertyDesc;
		}
	}
	return ReturnDesc; 
}

void UBTD_Base::RequestAbort(UBehaviorTreeComponent& OwnerComp, const EAbortType Type)
{
	switch (Type)
	{
	case EAbortType::NoAbort:
		// Nothing to abort, continue
			break;
	case EAbortType::ActivateBranch:
		OwnerComp.RequestBranchActivation(*this, false);
		break;
	case EAbortType::DeactivateBranch:
		OwnerComp.RequestBranchDeactivation(*this);
		break;
	case EAbortType::Unknown:
		checkf(false, TEXT("If decorator is active, it must know whether it needs to abort or not"))
		break;
	}
}

EBlackboardNotificationResult UBTD_Base::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard,
	FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
	if (BehaviorComp)
	{
		const EAbortType Type = EvaluateAbortType(*BehaviorComp);
		if (Type != EAbortType::Unknown)
		{
			RequestAbort(*BehaviorComp, Type);
		}
	}
	return BehaviorComp ? EBlackboardNotificationResult::ContinueObserving : EBlackboardNotificationResult::RemoveObserver;
}

void UBTD_Base::PostLoad()
{
	Super::PostLoad();
	
	if (GetFlowAbortMode() != EBTFlowAbortMode::None && bIsObservingBB)
	{
		ObservedKeyNames.Reset();
		UClass* StopAtClass = UBTD_Base::StaticClass();
		BlueprintNodeHelpers::CollectBlackboardSelectors(this, StopAtClass, ObservedKeyNames);
		ensure(ObservedKeyNames.Num() > 0);
	}
}

void UBTD_Base::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		for (int32 NameIndex = 0; NameIndex < ObservedKeyNames.Num(); NameIndex++)
		{
			const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(ObservedKeyNames[NameIndex]);
			if (KeyID != FBlackboard::InvalidKey)
			{
				BlackboardComp->RegisterObserver(KeyID, this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTD_Base::OnBlackboardKeyValueChange));
			}
		}
	}
}

UBTD_Base::EAbortType UBTD_Base::EvaluateAbortType(UBehaviorTreeComponent& OwnerComp) const
{
	// if (PerformConditionCheckImplementations == 0)
	// {
	// 	return EAbortType::Unknown;
	// }

	if (FlowAbortMode == EBTFlowAbortMode::None)
	{
		return EAbortType::NoAbort;
	}

	const bool bIsOnActiveBranch = OwnerComp.IsExecutingBranch(GetMyNode(), GetChildIndex());

	EAbortType AbortType = EAbortType::NoAbort;
	if (bIsOnActiveBranch)
	{
		if ((FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, /*NodeMemory*/nullptr) == IsInversed())
		{
			AbortType = EAbortType::DeactivateBranch;
		}
	}
	else 
	{
		if ((FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, /*NodeMemory*/nullptr) != IsInversed())
		{
			AbortType = EAbortType::ActivateBranch;
		}
	}

	return AbortType;
}
