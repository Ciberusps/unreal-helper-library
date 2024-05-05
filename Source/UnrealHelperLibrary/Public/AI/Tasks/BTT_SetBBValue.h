// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyEnums.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BTT_SetBBValue.generated.h"

UENUM(BlueprintType)
enum ESetBBValueTypes
{
	None,
	Bool,
	Int,
	Float,
	String,
	Name,
	Vector,
	Rotator,
	Enum,
	NativeEnum,
	Object,
	Class,
};

/**
 * Use with caution, setting BB values in BehaviorTrees directly is "anti-pattern"
 * mostly you want to write BTTasks where its possible, use it for test purposes
 *
 * TODO arithmetic operations like A + B, A * B ...
 */
UCLASS()
class UNREALHELPERLIBRARY_API UBTT_SetBBValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetBBValue(const FObjectInitializer& ObjectInitializer);
	
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector BlackboardKey;
	
	/** Values */
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Bool", EditConditionHides))
	bool BoolValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Int", EditConditionHides))
	int32 IntValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Float", EditConditionHides))
	float FloatValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::String", EditConditionHides))
	FString StringValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Name", EditConditionHides))
	FName NameValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Vector", EditConditionHides))
	FVector VectorValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Rotator", EditConditionHides))
	FRotator RotatorValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Enum || CurrentBBKeyValueType==ESetBBValueTypes::NativeEnum", EditConditionHides, GetOptions="GetEnumOptions"))
	FString EnumStringValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Object", EditConditionHides))
	FBlackboardKeySelector ObjectValue;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Value", EditCondition="CurrentBBKeyValueType==ESetBBValueTypes::Class", EditConditionHides))
	UClass* ClassValue;
	/** ~Values */
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
	
#if WITH_EDITORONLY_DATA

	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;
	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<EArithmeticKeyOperation::Type> ArithmeticOperation;
	// UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Key Query"))
	// TEnumAsByte<ETextKeyOperation::Type> TextOperation;

#endif

#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UFUNCTION()
	TArray<FString> GetEnumOptions();
	UPROPERTY()
	UEnum* CurrentEnum;
	UPROPERTY()
	TEnumAsByte<ESetBBValueTypes> CurrentBBKeyValueType = ESetBBValueTypes::None;
};
