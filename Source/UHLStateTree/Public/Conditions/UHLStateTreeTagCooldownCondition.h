// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "StateTreeConditionBase.h"
#include "UHLStateTreeTagCooldownCondition.generated.h"

USTRUCT()
struct UHLSTATETREE_API FUHLStateTreeTagCooldownConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag CooldownTag;
};

/**
 * HasTagCooldown condition
 */
USTRUCT(DisplayName="Has Tag Cooldown", Category="Gameplay Tags")
struct UHLSTATETREE_API FUHLStateTreeTagCooldownCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FUHLStateTreeTagCooldownConditionInstanceData;

	FUHLStateTreeTagCooldownCondition() = default;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Tag");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::DarkGrey;
	}
#endif
};
