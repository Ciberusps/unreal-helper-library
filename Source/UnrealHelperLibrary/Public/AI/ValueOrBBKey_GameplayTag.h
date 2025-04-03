// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "ValueOrBBKey_GameplayTag.generated.h"


USTRUCT(BlueprintType, Category = "UnrealHelperLibrary")
struct UNREALHELPERLIBRARY_API FValueOrBBKey_GameplayTag : public FValueOrBlackboardKeyBase
{
	GENERATED_BODY()

	FValueOrBBKey_GameplayTag(FGameplayTag Default = FGameplayTag::EmptyTag)
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
struct TStructOpsTypeTraits<FValueOrBBKey_GameplayTag> : public TStructOpsTypeTraitsBase2<FValueOrBBKey_GameplayTag>
{
	enum
	{
		WithStructuredSerializeFromMismatchedTag = true,
	};
};
