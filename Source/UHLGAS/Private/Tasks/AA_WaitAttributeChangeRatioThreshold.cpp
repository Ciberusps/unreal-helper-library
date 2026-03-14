// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/AA_WaitAttributeChangeRatioThreshold.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AA_WaitAttributeChangeRatioThreshold)

UAA_WaitAttributeChangeRatioThreshold::UAA_WaitAttributeChangeRatioThreshold(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTriggerOnce = false;
	bMatchedComparisonLastAttributeChange = false;
	LastAttributeNumeratorValue = 1.f;
	LastAttributeDenominatorValue = 1.f;
}

UAA_WaitAttributeChangeRatioThreshold* UAA_WaitAttributeChangeRatioThreshold::WaitForAttributeChangeRatioThreshold(AActor* TargetActor, FGameplayAttribute AttributeNumerator, FGameplayAttribute AttributeDenominator, TEnumAsByte<EWaitAttributeChangeComparison::Type> ComparisonType, float ComparisonValue, bool bTriggerOnce, AActor* OptionalExternalOwner)
{
	UAA_WaitAttributeChangeRatioThreshold* MyTask = NewObject<UAA_WaitAttributeChangeRatioThreshold>();
	MyTask->SetAbilityActor(TargetActor);
	MyTask->AttributeNumerator = AttributeNumerator;
	MyTask->AttributeDenominator = AttributeDenominator;
	MyTask->ComparisonType = ComparisonType;
	MyTask->ComparisonValue = ComparisonValue;
	MyTask->bTriggerOnce = bTriggerOnce;
	MyTask->ExternalOwner = OptionalExternalOwner ? UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OptionalExternalOwner) : nullptr;

	return MyTask;
}

void UAA_WaitAttributeChangeRatioThreshold::Activate()
{
	if (UAbilitySystemComponent* ASC = GetFocusedASC())
	{
		LastAttributeNumeratorValue = ASC->GetNumericAttribute(AttributeNumerator);
		LastAttributeDenominatorValue = ASC->GetNumericAttribute(AttributeDenominator);
		bMatchedComparisonLastAttributeChange = DoesValuePassComparison(LastAttributeNumeratorValue, LastAttributeDenominatorValue);

		// Broadcast OnChange immediately with current value
		if (ShouldBroadcastDelegates())
		{
			OnChange.Broadcast(bMatchedComparisonLastAttributeChange, LastAttributeDenominatorValue != 0.f ? LastAttributeNumeratorValue/LastAttributeDenominatorValue : 0.f);
		}

		OnNumeratorAttributeChangeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeNumerator).AddUObject(this, &UAA_WaitAttributeChangeRatioThreshold::OnNumeratorAttributeChange);
		OnDenominatorAttributeChangeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeDenominator).AddUObject(this, &UAA_WaitAttributeChangeRatioThreshold::OnDenominatorAttributeChange);
	}
	else
	{
		EndAction();
	}
}

void UAA_WaitAttributeChangeRatioThreshold::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetFocusedASC())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeNumerator).Remove(OnNumeratorAttributeChangeDelegateHandle);
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeDenominator).Remove(OnDenominatorAttributeChangeDelegateHandle);
	}

	Super::EndAction();
}

UAbilitySystemComponent* UAA_WaitAttributeChangeRatioThreshold::GetFocusedASC() const
{
	return ExternalOwner ? ToRawPtr(ExternalOwner) : GetAbilitySystemComponent();
}

void UAA_WaitAttributeChangeRatioThreshold::OnAttributeChange()
{
	UWorld* World = GetAbilitySystemComponent()->GetWorld();
	if (World && !CheckAttributeTimer.IsValid())
	{
		// Trigger OnRatioChange check at the end of this frame/next so that any individual attribute change
		// has time for the other attribute to update (in case they're linked)
		World->GetTimerManager().SetTimer(CheckAttributeTimer, this, &UAA_WaitAttributeChangeRatioThreshold::OnRatioChange, 0.001f, false);
	}
}

void UAA_WaitAttributeChangeRatioThreshold::OnRatioChange()
{
	CheckAttributeTimer.Invalidate();

	bool bPassedComparison = DoesValuePassComparison(LastAttributeNumeratorValue, LastAttributeDenominatorValue);
	if (bPassedComparison != bMatchedComparisonLastAttributeChange)
	{
		bMatchedComparisonLastAttributeChange = bPassedComparison;
		if (ShouldBroadcastDelegates())
		{
			OnChange.Broadcast(bMatchedComparisonLastAttributeChange, LastAttributeDenominatorValue != 0.f ? LastAttributeNumeratorValue/LastAttributeDenominatorValue : 0.f);
		}
		if (bTriggerOnce)
		{
			EndAction();
		}
	}
}

void UAA_WaitAttributeChangeRatioThreshold::OnNumeratorAttributeChange(const FOnAttributeChangeData& CallbackData)
{
	LastAttributeNumeratorValue = CallbackData.NewValue;
	OnAttributeChange();
}

void UAA_WaitAttributeChangeRatioThreshold::OnDenominatorAttributeChange(const FOnAttributeChangeData& CallbackData)
{
	LastAttributeDenominatorValue = CallbackData.NewValue;
	OnAttributeChange();
}

bool UAA_WaitAttributeChangeRatioThreshold::DoesValuePassComparison(float ValueNumerator, float ValueDenominator) const
{
	if (ValueDenominator == 0.f)
	{
		return bMatchedComparisonLastAttributeChange;
	}

	const float CurrentRatio = ValueNumerator / ValueDenominator;
	bool bPassedComparison = true;
	switch (ComparisonType)
	{
	case EWaitAttributeChangeComparison::ExactlyEqualTo:
		bPassedComparison = (CurrentRatio == ComparisonValue);
		break;		
	case EWaitAttributeChangeComparison::GreaterThan:
		bPassedComparison = (CurrentRatio > ComparisonValue);
		break;
	case EWaitAttributeChangeComparison::GreaterThanOrEqualTo:
		bPassedComparison = (CurrentRatio >= ComparisonValue);
		break;
	case EWaitAttributeChangeComparison::LessThan:
		bPassedComparison = (CurrentRatio < ComparisonValue);
		break;
	case EWaitAttributeChangeComparison::LessThanOrEqualTo:
		bPassedComparison = (CurrentRatio <= ComparisonValue);
		break;
	case EWaitAttributeChangeComparison::NotEqualTo:
		bPassedComparison = (CurrentRatio != ComparisonValue);
		break;
	default:
		break;
	}
	return bPassedComparison;
}
