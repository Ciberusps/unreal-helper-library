// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Data/TurnSettings.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TurnTo.generated.h"

class AAIController;

struct FBTTurnTo : FBTFocusMemory
{
    FTurnRange CurrentTurnRange;
    bool bCurrentTurnRangeSet = false;

    void Reset()
    {
        FocusActorSet = nullptr;
        FocusLocationSet = FAISystem::InvalidLocation;
        bActorSet = false;
        bCurrentTurnRangeSet = false;
    }
};

/**
 * Mostly copy-pasted from RotateToFaceBBEntry but can use animations to rotate
 * TODO bForceRotateNotOnlyWithAnims - overrides DataAsset setting, in rare cases we need to rotate in place
 */
UCLASS(config = Game, Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTT_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
    // TODO IMO can't be 0.0f, enemy wont turn such precisely clamp to 0.1f at least?
    /** Success condition precision in degrees */
    UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0", Units="Degrees"))
    float Precision = 1.0f;

    UPROPERTY(EditAnywhere)
    bool bUseTurnAnimations = true;
    UPROPERTY(EditAnywhere, meta=(EditCondition="bUseTurnAnimations", EditConditionHides))
    bool bUseTurnSettingsDataAsset = true;

    UPROPERTY(EditAnywhere, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides))
    FTurnSettings TurnSettings;
    UPROPERTY(EditAnywhere, meta=(EditCondition="bUseTurnAnimations && bUseTurnSettingsDataAsset", EditConditionHides))
    UTurnToAnimationsDataAsset* RotateToAnimationsDataAsset;

    UPROPERTY(EditAnywhere)
    bool bDebug = false;

    // Prefers to rotate 180deg if relative angle >115deg, suits for all medium mobs(human size)
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=1))
    void SetupPreset_Default_90_180();
    // All ranges have same proportion
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=1))
    void SetupPreset_BigEnemy_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=2))
    void SetupPreset_45_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=3))
    void SetupPreset_15_45_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=4))
    void SetupPreset_15_30_45_90_180();
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=5))
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

    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTTurnTo); }
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

protected:
    float GetPrecisionDot() const { return PrecisionDot; }
    void CleanUp(AAIController& AIController, uint8* NodeMemory);

private:
    bool IsTurnWithAnimationRequired(ACharacter* Character) const;
    bool CanStopMontage(uint8* NodeMemory);
    FTurnRange GetTurnRange(float DeltaAngle, bool& bCurrentTurnRangeSet);
    FTurnSettings* GetTurnSettings();
};
