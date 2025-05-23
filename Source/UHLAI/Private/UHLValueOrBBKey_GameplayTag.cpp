// Pavel Penkov 2025 All Rights Reserved.


#include "UHLValueOrBBKey_GameplayTag.h"

#include "GameplayTagContainer.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLValueOrBBKey_GameplayTag)

FGameplayTag FUHLValueOrBBKey_GameplayTag::GetValue(const UBlackboardComponent& Blackboard) const
{
	FName TagName = FBlackboard::GetValue<UBlackboardKeyType_Name>(Blackboard, Key, KeyId, DefaultValue.GetTagName());
	FGameplayTag FoundGameplayTag = FGameplayTag::RequestGameplayTag(TagName, false);
	if (!FoundGameplayTag.IsValid())
	{
		UUnrealHelperLibraryBPL::DebugPrintString(Blackboard.GetWorld(),
			FString::Printf(TEXT("FUHLValueOrBBKey_GameplayTag::GetValue GameplayTag %s not found"), *Key.ToString())
		);
		return DefaultValue;
	}
	return FoundGameplayTag;
}

FGameplayTag FUHLValueOrBBKey_GameplayTag::GetValue(const UBlackboardComponent* Blackboard) const
{
	return Blackboard ? GetValue(*Blackboard) : DefaultValue;
}

FGameplayTag FUHLValueOrBBKey_GameplayTag::GetValue(const UBehaviorTreeComponent& BehaviorComp) const
{
	FName TagName = FBlackboard::GetValue<UBlackboardKeyType_Name>(BehaviorComp, Key, KeyId, DefaultValue.GetTagName());
	FGameplayTag FoundGameplayTag = FGameplayTag::RequestGameplayTag(TagName, false);
	if (!FoundGameplayTag.IsValid())
	{
		UUnrealHelperLibraryBPL::DebugPrintString(BehaviorComp.GetWorld(),
			FString::Printf(TEXT("FUHLValueOrBBKey_GameplayTag::GetValue GameplayTag %s not found"), *Key.ToString())
		);
		return DefaultValue;
	}
	return FoundGameplayTag;
}

FGameplayTag FUHLValueOrBBKey_GameplayTag::GetValue(const UBehaviorTreeComponent* BehaviorComp) const
{
	return BehaviorComp ? GetValue(*BehaviorComp) : DefaultValue;
}

bool FUHLValueOrBBKey_GameplayTag::SerializeFromMismatchedTag(
	const FPropertyTag& Tag, FStructuredArchive::FSlot Slot)
{
	if (Tag.Type == NAME_StructProperty && Tag.GetType().IsStruct(FGameplayTag::StaticStruct()->GetFName()))
	{
		FGameplayTag TempTag;
		FGameplayTag::StaticStruct()->SerializeItem(Slot, &TempTag, nullptr);
		// Slot << TempTag; // Deserialize the data as an FGameplayTag
		DefaultValue = TempTag; // Store it in a compatible property
		return true;
	}
	return false;	
}

#if WITH_EDITOR
bool FUHLValueOrBBKey_GameplayTag::IsCompatibleType(const UBlackboardKeyType* KeyType) const
{
	return KeyType && KeyType->GetClass() == UBlackboardKeyType_Name::StaticClass();
}
#endif

FString FUHLValueOrBBKey_GameplayTag::ToString() const
{
	if (!Key.IsNone())
	{
		return ToStringKeyName();
	}
	else
	{
		return DefaultValue.GetTagName().ToString();
	}
}
