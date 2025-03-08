// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/AN_FireGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

#if WITH_EDITOR
void UAN_FireGameplayEvent::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && 
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAN_FireGameplayEvent, EventTag))
	{
		if (UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(GetOuter()))
		{
			AnimSeq->Modify();
		}
	}
}
#endif

FString UAN_FireGameplayEvent::GetNotifyName_Implementation() const
{
	return FString("FireGameplayEvent->") + EventTag.ToString();
}

void UAN_FireGameplayEvent::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	
	AActor* TargetActor = MeshComp->GetOwner();
	if (!TargetActor) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	FGameplayEventData EventData;
	EventData.Target = TargetActor;
	EventData.Instigator = TargetActor;
	EventData.EventMagnitude = EventMagnitude;
	EventData.OptionalObject = OptionalObject;
	EventData.OptionalObject2 = OptionalObject2;
	EventData.TargetTags = TargetTags;
	EventData.InstigatorTags = InstigatorTags;
	TargetASC->HandleGameplayEvent(EventTag, &EventData);
}