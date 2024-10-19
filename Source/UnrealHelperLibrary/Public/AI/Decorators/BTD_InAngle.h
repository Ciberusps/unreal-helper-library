// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "AI/Decorators/BTD_Base.h"
#include "BTD_InAngle.generated.h"

struct FBTInAngleMemory
{
};

USTRUCT(BlueprintType)
struct FAngleRange
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="Decorator", EditAnywhere)
	FFloatRange Range = FFloatRange();

	// ~"FColor::MakeRandomColor()" will lead to non-critical error
	// ~unreal don't support random colors from native code.
	// ~They should be deterministic but there is no option
	UPROPERTY(Category="Decorator", EditAnywhere)
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

	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="", EditConditionHides))
	FBlackboardKeySelector Target;
	UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="", EditConditionHides))
	TArray<FAngleRange> IncludeRanges;
	// UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="", EditConditionHides))
	// TArray<FAngleRange> ExcludeRanges;

    // TODO bIncludeTargetCharacterCapsule - checks not only angle to actor
    // but also that TargetCharacter capsule in angle

	UPROPERTY(Category="Decorator", EditAnywhere)
	bool bDrawDebug = false;
    // helpful for testing ranges
    UPROPERTY(Category="Decorator", EditAnywhere)
    bool bDebugForceFalseCondition = false;
    UPROPERTY(Category="Decorator", EditAnywhere)
    float OverrideDebugLinesDistance = 0.0f;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTInAngleMemory); };
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

	virtual FString GetStaticDescription() const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	bool IsInAngle(float CurrentAngle) const;

private:
	TWeakObjectPtr<ACharacter> CachedOwnerCharacter;

	float GetCurrentAngle(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, bool bDrawDebug_In = false) const;
};
