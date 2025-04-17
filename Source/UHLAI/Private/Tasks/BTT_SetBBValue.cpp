// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/BTT_SetBBValue.h"

#include "UHLAIBlueprintLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Utils/UnrealHelperLibraryBPL.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_SetBBValue)

UBTT_SetBBValue::UBTT_SetBBValue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ObjectValue.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_SetBBValue, ObjectValue), UObject::StaticClass());
}

EBTNodeResult::Type UBTT_SetBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardAsset) return EBTNodeResult::Failed;

	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);

	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;


	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		BlackboardComponent->SetValueAsBool(BlackboardKey.SelectedKeyName, BoolValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		int32 CurrentValue = BlackboardComponent->GetValueAsInt(BlackboardKey.SelectedKeyName);
	    int32 Result = OperationOnBBValue.CalculateIntResult(OwnerComp, CurrentValue);
		BlackboardComponent->SetValueAsInt(BlackboardKey.SelectedKeyName, Result);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		float CurrentValue = BlackboardComponent->GetValueAsFloat(BlackboardKey.SelectedKeyName);
		float Result = OperationOnBBValue.CalculateFloatResult(OwnerComp, CurrentValue);
		BlackboardComponent->SetValueAsFloat(BlackboardKey.SelectedKeyName, Result);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_String::StaticClass())
	{
		BlackboardComponent->SetValueAsString(BlackboardKey.SelectedKeyName, StringValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Name::StaticClass())
	{
		BlackboardComponent->SetValueAsName(BlackboardKey.SelectedKeyName, NameValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		FVector CurrentValue = BlackboardComponent->GetValueAsVector(BlackboardKey.SelectedKeyName);
		FVector Result = OperationOnBBValue.CalculateVectorResult(OwnerComp, CurrentValue);
		BlackboardComponent->SetValueAsVector(BlackboardKey.SelectedKeyName, Result);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		FRotator CurrentValue = BlackboardComponent->GetValueAsRotator(BlackboardKey.SelectedKeyName);
		FRotator Result = OperationOnBBValue.CalculateRotatorResult(OwnerComp, CurrentValue);
		BlackboardComponent->SetValueAsRotator(BlackboardKey.SelectedKeyName, Result);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
	{
		CurrentEnum = ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType;
	    if (CurrentEnum)
	    {
	        BlackboardComponent->SetValueAsEnum(BlackboardKey.SelectedKeyName, CurrentEnum->GetValueByNameString(EnumStringValue));
	    }
	    else
	    {
	        UUnrealHelperLibraryBPL::DebugPrintStrings("Enum from BB key ", BlackboardKey.SelectedKeyName.ToString(), "not found");
	    }
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		CurrentEnum = ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;
	    if (CurrentEnum)
	    {
	        BlackboardComponent->SetValueAsEnum(BlackboardKey.SelectedKeyName, CurrentEnum->GetValueByNameString(EnumStringValue));
	    }
		else
		{
			UUnrealHelperLibraryBPL::DebugPrintStrings("Enum from BB key ", BlackboardKey.SelectedKeyName.ToString(), "not found");
		}
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		if (ObjectValue.NeedsResolving())
		{
			ObjectValue.ResolveSelectedKey(*BlackboardAsset);
		}
		BlackboardComponent->SetValueAsObject(BlackboardKey.SelectedKeyName, BlackboardComponent->GetValueAsObject(ObjectValue.SelectedKeyName));
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Class::StaticClass())
	{
		BlackboardComponent->SetValueAsClass(BlackboardKey.SelectedKeyName, ClassValue);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UBTT_SetBBValue::GetStaticDescription() const
{
	FString Description = FString();

	switch (CurrentBBKeyValueType)
	{
		case EBlackboardValueType::Bool:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), BoolValue ? TEXT("true") : TEXT("false"));
			break;
		case EBlackboardValueType::Int:
			Description = OperationOnBBValue.ToStringIntValue(BlackboardKey.SelectedKeyName.ToString());
			break;
		case EBlackboardValueType::Float:
			Description = OperationOnBBValue.ToStringFloatValue(BlackboardKey.SelectedKeyName.ToString());
			break;
		case EBlackboardValueType::String:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *StringValue);
			break;
		case EBlackboardValueType::Name:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *NameValue.ToString());
			break;
		case EBlackboardValueType::Vector:
			Description = OperationOnBBValue.ToStringVectorValue(BlackboardKey.SelectedKeyName.ToString());
			break;
		case EBlackboardValueType::Rotator:
			Description = OperationOnBBValue.ToStringRotatorValue(BlackboardKey.SelectedKeyName.ToString());
			break;
		case EBlackboardValueType::Enum:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			break;
		case EBlackboardValueType::NativeEnum:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			break;
		case EBlackboardValueType::Object:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *ObjectValue.SelectedKeyName.ToString());
			break;
		case EBlackboardValueType::Class:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), ClassValue ? *ClassValue->GetName() : TEXT(""));
			break;
		default:
			break;
	}

	// return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), *Description);
	return FString::Printf(TEXT("%s"), *Description);
}

void UBTT_SetBBValue::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	BlackboardKey.ResolveSelectedKey(*Asset.BlackboardAsset);

	CurrentBBKeyValueType = UUHLAIBlueprintLibrary::BlackboardKeyToBBValueType(BlackboardKey);
	OperationOnBBValue.SetCurrentBBKeyValueType(CurrentBBKeyValueType);
}

void UBTT_SetBBValue::PostLoad()
{
	Super::PostLoad();

	// migrate to FOperationOnBBValue
	if (!bMigratedToMathOperation)
	{
		if (MathOperation != EUHL_MathOperations::None)
		{
			OperationOnBBValue.MathOperation = MathOperation;
			MathOperation = EUHL_MathOperations::None;
		}
		if (IntValue != 0)
		{
			OperationOnBBValue.IntValue = IntValue;
			IntValue = 0;
		};
		if (FloatValue != 0)
		{
			OperationOnBBValue.FloatValue = FloatValue;
			FloatValue = 0;
		};
		if (VectorValue != FVector::ZeroVector)
		{
			OperationOnBBValue.VectorValue = VectorValue;
			VectorValue = FVector::ZeroVector;
		};
		if (RotatorValue != FRotator::ZeroRotator)
		{
			OperationOnBBValue.RotatorValue = RotatorValue;
			RotatorValue = FRotator::ZeroRotator;
		};
		
		bMigratedToMathOperation = true;
	}
}

#if WITH_EDITOR
void UBTT_SetBBValue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	if (!BlackboardAsset) return;

	if (!BlackboardKey.IsSet()) return;
	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);

	CurrentBBKeyValueType = UUHLAIBlueprintLibrary::BlackboardKeyToBBValueType(BlackboardKey);
	OperationOnBBValue.SetCurrentBBKeyValueType(CurrentBBKeyValueType);

	// if (CurrentBBKeyValueType == EBlackboardValueType::Int)
	// {
	// 	MathOperation = EUHL_MathOperations::Set;
	// }
}
#endif

TArray<FString> UBTT_SetBBValue::GetEnumOptions()
{
	TArray<FString> Result = {};

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	if (!BlackboardAsset) return Result;

	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);
	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

	if (BlackboardKey.SelectedKeyType != UBlackboardKeyType_Enum::StaticClass()
		|| BlackboardKey.SelectedKeyType != UBlackboardKeyType_NativeEnum::StaticClass())
	{
		return Result;
	}
	
	if (!EntryInfo) return Result;

	const UEnum* Enum = (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
		                    ? ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType
		                    : ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;

	if (!Enum || !Enum->IsValidLowLevel())
	{
		return Result;
	}

	for (int32 i = 0; i < Enum->NumEnums(); i++)
	{
		Result.Add(Enum->GetNameByIndex(i).ToString());
	}

	return Result;
}
