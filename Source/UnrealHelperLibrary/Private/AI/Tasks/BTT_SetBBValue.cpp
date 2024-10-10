// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_SetBBValue.h"

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
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"

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
	    int32 ResultInt = 0;
	    if (IsMathOperationRequired())
	    {
	        ResultInt = MathOperation == ESetBBValue_MathOperations::Set
	            ? IntValue
	            : BlackboardComponent->GetValueAsInt(BlackboardKey.SelectedKeyName);
		    if (MathOperation == ESetBBValue_MathOperations::Add)
		    {
		        ResultInt += IntValue;
		    }
		    else if (MathOperation == ESetBBValue_MathOperations::Multiply)
		    {
		        ResultInt *= IntValue;
		    }
		    else if (MathOperation == ESetBBValue_MathOperations::Divide)
		    {
		        ResultInt /= IntValue;
		    }
	    }
		BlackboardComponent->SetValueAsInt(BlackboardKey.SelectedKeyName, ResultInt);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		BlackboardComponent->SetValueAsFloat(BlackboardKey.SelectedKeyName, FloatValue);
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
		BlackboardComponent->SetValueAsVector(BlackboardKey.SelectedKeyName, VectorValue);
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		BlackboardComponent->SetValueAsRotator(BlackboardKey.SelectedKeyName, RotatorValue);
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
		case EBBValueType::Bool:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), BoolValue ? TEXT("true") : TEXT("false"));
			break;
		case EBBValueType::Int:
			Description = FString::Printf(TEXT("Set \"%s\" to %d"), *BlackboardKey.SelectedKeyName.ToString(), IntValue);
			break;
		case EBBValueType::Float:
			Description = FString::Printf(TEXT("Set \"%s\" to %f"), *BlackboardKey.SelectedKeyName.ToString(), FloatValue);
			break;
		case EBBValueType::String:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *StringValue);
			break;
		case EBBValueType::Name:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *NameValue.ToString());
			break;
		case EBBValueType::Vector:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *VectorValue.ToString());
			break;
		case EBBValueType::Rotator:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *RotatorValue.ToString());
			break;
		case EBBValueType::Enum:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			break;
		case EBBValueType::NativeEnum:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *EnumStringValue);
			break;
		case EBBValueType::Object:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), *ObjectValue.SelectedKeyName.ToString());
			break;
		case EBBValueType::Class:
			Description = FString::Printf(TEXT("Set \"%s\" to %s"), *BlackboardKey.SelectedKeyName.ToString(), ClassValue ? *ClassValue->GetName() : TEXT(""));
			break;
		default:
			break;
	}

	// return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), *Description);
	return FString::Printf(TEXT("%s"), *Description);
}

#if WITH_EDITOR
void UBTT_SetBBValue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	if (!BlackboardAsset) return;

	if (!BlackboardKey.IsSet()) return;
	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);
	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

	CurrentBBKeyValueType = UUnrealHelperLibraryBPL::BlackboardKeyToBBValueType(BlackboardKey);
    MathOperation = ESetBBValue_MathOperations::Set;
}
#endif

TArray<FString> UBTT_SetBBValue::GetEnumOptions()
{
	TArray<FString> Result = {};

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	if (!BlackboardAsset) return Result;

	BlackboardKey.ResolveSelectedKey(*BlackboardAsset);
	const FBlackboardEntry* EntryInfo = BlackboardAsset ? BlackboardAsset->GetKey(BlackboardKey.GetSelectedKeyID()) : NULL;

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
