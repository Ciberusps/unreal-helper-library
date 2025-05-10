// Pavel Penkov 2025 All Rights Reserved.

#include "Conditions/UHLStateTreeTagCooldownCondition.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "Components/UHLStateTreeAIComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLStateTreeTagCooldownCondition)

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "UHLStateTree"

namespace UE::StateTree::Conditions
{


}

#endif// WITH_EDITOR

bool FUHLStateTreeTagCooldownCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController) return false;
	
	if (UUHLStateTreeAIComponent* Cmp = Cast<UUHLStateTreeAIComponent>(AIController->GetBrainComponent()))
	{
		return Cmp->TagCooldowns.HasCooldownFinished(Context.GetOwner(), InstanceData.CooldownTag);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("[UHLStateTreeSetCooldownTask] using UUHLStateTreeAIComponent required to use SetCooldownTask"));
	}
	
	return false;
}

#if WITH_EDITOR
FText FUHLStateTreeTagCooldownCondition::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("GameplayTagMatchRich", "Has cooldown with {CooldownTag}")
		: LOCTEXT("GameplayTagMatch", "Has cooldown with {CooldownTag}");

	return FText::FormatNamed(Format,
		TEXT("CooldownTag"), FText::FromString(InstanceData->CooldownTag.ToString()));
}
#endif
