// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/BTT_DebugPrintBBValue.h"

#include "UHLAIBlueprintLibrary.h"
#include "Utils/UnrealHelperLibraryBPL.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_DebugPrintBBValue)

UBTT_DebugPrintBBValue::UBTT_DebugPrintBBValue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "DebugPrintBBValue";
}

EBTNodeResult::Type UBTT_DebugPrintBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UUnrealHelperLibraryBPL::DebugPrintStrings(GetBBKeyDescription(OwnerComp), "", "", "", "", "","","","","",Duration, Key, true);
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UBTT_DebugPrintBBValue::GetBBKeyDescription(UBehaviorTreeComponent& OwnerComp)
{
	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);

	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	EBBValueType BBValueType = UUHLAIBlueprintLibrary::BlackboardKeyToBBValueType(BlackboardKey);

	FString Description = FString();

	switch (BBValueType)
	{
		case EBBValueType::Bool:
			{
				bool BoolValue = BlackboardComponent->GetValueAsBool(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %hhd"), *BlackboardKey.SelectedKeyName.ToString(), BoolValue);
			}
			break;
		case EBBValueType::Int:
			{
				int32 IntValue = BlackboardComponent->GetValueAsInt(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %d"), *BlackboardKey.SelectedKeyName.ToString(), IntValue);
			}
			break;
		case EBBValueType::Float:
			{
				float FloatValue = BlackboardComponent->GetValueAsFloat(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %f"), *BlackboardKey.SelectedKeyName.ToString(), FloatValue);
			}
			break;
		case EBBValueType::String:
			{
				FString StringValue = BlackboardComponent->GetValueAsString(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *StringValue);
			}
			break;
		case EBBValueType::Name:
			{
				FName NameValue = BlackboardComponent->GetValueAsName(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *NameValue.ToString());
			}
			break;
		case EBBValueType::Vector:
			{
				FVector VectorValue = BlackboardComponent->GetValueAsVector(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *VectorValue.ToString());
			}
			break;
		case EBBValueType::Rotator:
			{
				FRotator RotatorValue = BlackboardComponent->GetValueAsRotator(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *RotatorValue.ToString());
			}
			break;
		case EBBValueType::Enum:
			{
				UEnum* Enum = (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
							   ? ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType
							   : ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;
				uint8 EnumValue = BlackboardComponent->GetValueAsEnum(BlackboardKey.SelectedKeyName);

				FString EnumStringValue = Enum->GetNameStringByValue(EnumValue);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			}
			break;
		case EBBValueType::NativeEnum:
			{
				UEnum* Enum = (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
							   ? ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType
							   : ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;
				uint8 EnumValue = BlackboardComponent->GetValueAsEnum(BlackboardKey.SelectedKeyName);
				FString EnumStringValue = Enum->GetNameStringByValue(EnumValue);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			}
			break;
		case EBBValueType::Object:
			{
				FBlackboardKeySelector ObjectValue = BlackboardKey;
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), *ObjectValue.SelectedKeyName.ToString());
			}
			break;
		case EBBValueType::Class:
			{
				UClass* ClassValue = BlackboardComponent->GetValueAsClass(BlackboardKey.SelectedKeyName);
				Description = FString::Printf(TEXT("\"%s\" is %s"), *BlackboardKey.SelectedKeyName.ToString(), ClassValue ? *ClassValue->GetName() : TEXT(""));
			}
			break;

		default:
			break;
	}

	return FString::Printf(TEXT("%s"), *Description);
}

FString UBTT_DebugPrintBBValue::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *BlackboardKey.SelectedKeyName.ToString());
}
