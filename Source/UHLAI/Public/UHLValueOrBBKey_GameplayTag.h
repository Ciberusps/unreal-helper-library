// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "UHLValueOrBBKey_GameplayTag.generated.h"

// TODO: Looks like there is same thing(not sure) in 
// Experimental/GameplayBehaviors/Source/GameplayBehaviorsModule/Public/AI/ValueOrBBKey_GameplayTag.h

USTRUCT(BlueprintType, Category = "UnrealHelperLibrary")
struct UHLAI_API FUHLValueOrBBKey_GameplayTag : public FValueOrBlackboardKeyBase
{
	GENERATED_BODY()

	FUHLValueOrBBKey_GameplayTag(FGameplayTag Default = FGameplayTag::EmptyTag)
		: DefaultValue(Default) {}
	FGameplayTag GetValue(const UBehaviorTreeComponent& BehaviorComp) const;
	FGameplayTag GetValue(const UBehaviorTreeComponent* BehaviorComp) const;
	FGameplayTag GetValue(const UBlackboardComponent& Blackboard) const;
	FGameplayTag GetValue(const UBlackboardComponent* Blackboard) const;

	bool SerializeFromMismatchedTag(const FPropertyTag& Tag, FStructuredArchive::FSlot Slot);

#if WITH_EDITOR
	virtual bool IsCompatibleType(const UBlackboardKeyType* KeyType) const override;
#endif // WITH_EDITOR

	FString ToString() const;

	UE_DEPRECATED_FORGAME(5.5, "Implicit conversion will be removed next version. Call GetValue instead")
	operator FName() const { return DefaultValue.GetTagName(); }

protected:
	UPROPERTY(EditAnywhere, Category = "Value")
	FGameplayTag DefaultValue = FGameplayTag::EmptyTag;
};

template <>
struct TStructOpsTypeTraits<FUHLValueOrBBKey_GameplayTag> : public TStructOpsTypeTraitsBase2<FUHLValueOrBBKey_GameplayTag>
{
	enum
	{
		WithStructuredSerializeFromMismatchedTag = true,
	};
};
