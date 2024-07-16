// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Data/TurnSettings.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TurnTo.generated.h"

class AAIController;

/**
 * Mostly copy-pasted from RotateToFaceBBEntry but can use animations to rotate
 */
UCLASS(config = Game)
class UNREALHELPERLIBRARY_API UBTT_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
    /** Success condition precision in degrees */
    UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0"))
    float Precision;

    UPROPERTY(EditAnywhere)
    bool bUseTurnAnimations = true;
    UPROPERTY(EditAnywhere, meta=(EditCondition="bUseTurnAnimations", EditConditionHides))
    bool bUseTurnSettingsDataAsset = true;

    UPROPERTY(EditAnywhere, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides))
    FTurnSettings TurnSettings;
    UPROPERTY(EditAnywhere, meta=(EditCondition="bUseTurnSettingsDataAsset", EditConditionHides))
    UTurnToAnimationsDataAsset* RotateToAnimationsDataAsset;

    UPROPERTY(EditAnywhere)
    bool bDebug = false;

    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=1))
    void SetupPreset_Recommended_90_180();
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

    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTFocusMemory); }
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

protected:

    float GetPrecisionDot() const { return PrecisionDot; }
    void CleanUp(AAIController& AIController, uint8* NodeMemory);

private:

    bool IsPlayingTurnMontage = false;
    UPROPERTY()
    UAnimMontage* CurrentTurnAnimMontage = nullptr;

    bool IsTurnWithAnimationRequired(ACharacter* Character) const;
    UAnimMontage* GetTurnAnimation(float DeltaAngle) const;
    FTurnSettings GetTurnSettings() const;
};
