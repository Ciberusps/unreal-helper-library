// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Decorators/BTD_Base.h"
#include "BTD_InAngle.generated.h"

struct FBTInAngleMemory
{
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	TWeakObjectPtr<ACharacter> TargetCharacter;
};

USTRUCT()
struct FAngleRange
{
	GENERATED_BODY()

public:
	// TODO on create random color;
	UPROPERTY(Category=Decorator, EditAnywhere)
	FFloatRange Range;
	UPROPERTY(Category=Decorator, EditAnywhere)
	FColor DebugColor = FColor::Blue;
};

/**
 * 
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTD_InAngle : public UBTD_Base
{
	GENERATED_BODY()

public:
	UBTD_InAngle(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="", EditConditionHides))
	FBlackboardKeySelector Target;
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="", EditConditionHides))
	TArray<FAngleRange> IncludeRanges;
	// UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="", EditConditionHides))
	// TArray<FAngleRange> ExcludeRanges;

	UPROPERTY(Category=Decorator, EditAnywhere)
	bool bDrawDebug = false;
	
	// virtual void SetOwner(AActor* ActorOwner) override;
	// virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual uint16 GetInstanceMemorySize() const override;
	// virtual FString GetStaticDescription() const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	float GetCurrentAngle(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, bool bDrawDebug_In = false) const;
	bool IsInAngle(float CurrentAngle) const;

	// virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	TWeakObjectPtr<ACharacter> CachedOwnerCharacter;

	void TryCacheTargetCharacter(const UBlackboardComponent* BlackboardComponent, FBTInAngleMemory* NodeMemory) const;
};
