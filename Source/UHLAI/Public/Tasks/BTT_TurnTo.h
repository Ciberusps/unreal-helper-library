// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/TurnSettings.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UnrealHelperLibraryTypes.h"
#include "GameFramework/Character.h"
#include "BTT_TurnTo.generated.h"

class AAIController;
class ACharacter;

namespace TurnToStatics
{
	static bool IsTurnWithAnimationRequired(ACharacter* Character)
	{
		if (!Character) return false;
		if (Character->IsPlayingRootMotion()) return false;
		return true;
	}

	static FTurnRange GetTurnRange(float DeltaAngle, bool& bCurrentTurnRangeSet, FTurnSettings TurnSettings_In)
	{
		FTurnRange Result;
		bCurrentTurnRangeSet = false;
		for (TTuple<FString, FTurnRanges> TurnToRange : TurnSettings_In.TurnRangesGroups)
		{
			for (FTurnRange Range : TurnToRange.Value.TurnRanges)
			{
				if (Range.Range.Contains(DeltaAngle))
				{
					Result = Range;
					bCurrentTurnRangeSet = true;
					break;
				}
			}
			if (bCurrentTurnRangeSet)
			{
				break;
			}
		}
		return Result;
	}

	FORCEINLINE_DEBUGGABLE FVector::FReal CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
	}
}

struct FBTTurnToMemory : FBTFocusMemory
{
    FTurnRange CurrentTurnRange;
    bool bCurrentTurnRangeSet = false;
    FTurnSettings TurnSettings;
    bool bCurrentTurnSettingsSet = false;

    void Reset()
    {
        FocusActorSet = nullptr;
        FocusLocationSet = FAISystem::InvalidLocation;
        bActorSet = false;
        bCurrentTurnRangeSet = false;
        bCurrentTurnSettingsSet = false;
    }
};

/**
 * Mostly copy-pasted from RotateToFaceBBEntry but can use animations to rotate
 * TODO bForceRotateNotOnlyWithAnims - overrides DataAsset setting, in rare cases we need to rotate in place
 */
UCLASS(config = Game, Category = "UnrealHelperLibrary")
class UHLAI_API UBTT_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
    // TODO IMO can't be 0.0f, enemy wont turn such precisely clamp to 0.1f at least?
    /** Success condition precision in degrees */
    UPROPERTY(config, Category="TurnTo", EditAnywhere, meta = (ClampMin = "0.0", Units="Degrees"))
    float Precision = 1.0f;

    UPROPERTY(EditAnywhere, Category="TurnTo")
    bool bUseTurnAnimations = true;
    UPROPERTY(EditAnywhere, Category="TurnTo", meta=(EditCondition="bUseTurnAnimations", EditConditionHides))
    EUHLSettingsSource SettingsSource = EUHLSettingsSource::Actor;

    UPROPERTY(EditAnywhere, Category="TurnTo", meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides))
    FTurnSettings TurnSettings;
    UPROPERTY(EditAnywhere, Category="TurnTo", meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::DataAsset", EditConditionHides))
    UTurnSettingsDataAsset* RotateToAnimationsDataAsset;

    UPROPERTY(EditAnywhere, Category="TurnTo")
    bool bDebug = false;

    // Prefers to rotate 180deg if relative angle >115deg, suits for all medium mobs(human size)
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides, DisplayPriority=1))
    void SetupPreset_Default_90_180();
    // All ranges have same proportion
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides, DisplayPriority=1))
    void SetupPreset_BigEnemy_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides, DisplayPriority=2))
    void SetupPreset_45_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides, DisplayPriority=3))
    void SetupPreset_15_45_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides, DisplayPriority=4))
    void SetupPreset_15_30_45_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && SettingsSource==EUHLSettingsSource::Node", EditConditionHides, DisplayPriority=5))
    void Cleanup();

private:
    /** cached Precision tangent value */
    float PrecisionDot;

public:
    UBTT_TurnTo(const FObjectInitializer& ObjectInitializer);

    virtual void PostInitProperties() override;
    virtual void PostLoad() override;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
    virtual FString GetStaticDescription() const override;

    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTTurnToMemory); }
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

protected:
    float GetPrecisionDot() const { return PrecisionDot; }
    void CleanUp(AAIController& AIController, uint8* NodeMemory);

private:
    FTurnSettings GetTurnSettings(AActor* Actor, bool& bCurrentTurnSettingsSet);
};
