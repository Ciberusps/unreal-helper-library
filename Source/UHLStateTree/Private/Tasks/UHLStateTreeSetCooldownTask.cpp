// Pavel Penkov 2025 All Rights Reserved.

#include "Tasks/UHLStateTreeSetCooldownTask.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/UHLStateTreeAIComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLStateTreeSetCooldownTask)

#define LOCTEXT_NAMESPACE "UHLStateTree"

EStateTreeRunStatus FUHLStateTreeSetCooldownTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	const UWorld* World = Context.GetWorld();
	// if (World == nullptr && InstanceData.ReferenceActor != nullptr)
	// {
	// 	World = InstanceData.ReferenceActor->GetWorld();
	// }

	// Reference actor is not required (offset will be used as a global world location)
	// but a valid world is required.
	if (World == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}

	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController) return EStateTreeRunStatus::Failed;
	
	if (UUHLStateTreeAIComponent* Cmp = Cast<UUHLStateTreeAIComponent>(AIController->GetBrainComponent()))
	{
		Cmp->TagCooldowns.AddCooldownTagDuration(Context.GetOwner(), InstanceData.CooldownTag, InstanceData.Duration, InstanceData.bAddToExistingDuration);
		if (InstanceData.bFinishTask)
		{
			return EStateTreeRunStatus::Succeeded;
		}
		else
		{
			return EStateTreeRunStatus::Running;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("[UHLStateTreeSetCooldownTask] using UUHLStateTreeAIComponent required to use SetCooldownTask"));
	return EStateTreeRunStatus::Failed;
}

#if WITH_EDITOR
FText FUHLStateTreeSetCooldownTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);
	
	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("DebugTextRich", "<b>Set Cooldown Tag</> \"{Text}\"")
		: LOCTEXT("DebugText", "Set Cooldown Tag \"{Text}\"");

	return FText::FormatNamed(Format, TEXT("Text"), FText::FromString(InstanceData->CooldownTag.ToString()));
}
#endif

#undef LOCTEXT_NAMESPACE