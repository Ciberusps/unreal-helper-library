// Pavel Penkov 2025 All Rights Reserved.


#include "AI/Tasks/BTT_FireGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_FireGameplayEvent)

UBTT_FireGameplayEvent::UBTT_FireGameplayEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FireGameplayEvent, Target), AActor::StaticClass());
	Instigator.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FireGameplayEvent, Instigator), AActor::StaticClass());

	Target.SelectedKeyName = FBlackboard::KeySelf;
	Instigator.SelectedKeyName = FBlackboard::KeySelf;
	
	OptionalObject.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FireGameplayEvent, OptionalObject), UObject::StaticClass());
	OptionalObject.AllowNoneAsValue(true);
	OptionalObject2.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FireGameplayEvent, OptionalObject2), UObject::StaticClass());
	OptionalObject2.AllowNoneAsValue(true);
}

EBTNodeResult::Type UBTT_FireGameplayEvent::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardAsset) return EBTNodeResult::Failed;
	
	Target.ResolveSelectedKey(*BlackboardAsset);
	Instigator.ResolveSelectedKey(*BlackboardAsset);
	OptionalObject.ResolveSelectedKey(*BlackboardAsset);
	OptionalObject2.ResolveSelectedKey(*BlackboardAsset);

	if (!Target.IsSet() || !Instigator.IsSet())
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetValue = BlackboardComponent->GetValueAsObject(Target.SelectedKeyName);
	UObject* InstigatorValue = BlackboardComponent->GetValueAsObject(Instigator.SelectedKeyName);
	if (!TargetValue || !InstigatorValue) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(TargetValue);
	AActor* InstigatorActor = Cast<AActor>(InstigatorValue);
	if (!TargetActor || !InstigatorActor) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	FGameplayEventData EventData;
	EventData.Target = TargetActor;
	EventData.Instigator = InstigatorActor;
	EventData.EventMagnitude = EventMagnitude;
	EventData.OptionalObject = BlackboardComponent->GetValueAsObject(OptionalObject.SelectedKeyName);
	EventData.OptionalObject2 = BlackboardComponent->GetValueAsObject(OptionalObject2.SelectedKeyName);
	EventData.TargetTags = TargetTags;
	EventData.InstigatorTags = InstigatorTags;
	TargetASC->HandleGameplayEvent(EventTag, &EventData);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UBTT_FireGameplayEvent::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}

