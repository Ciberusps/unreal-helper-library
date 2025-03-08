// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TurnSettings.generated.h"

class UAnimMontage;

USTRUCT(Blueprintable)
struct FTurnRange
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnRange")
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnRange")
    FFloatRange Range = FFloatRange(-135, -45);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnRange")
    UAnimMontage* AnimMontage = nullptr;
    // useful in big enemies cases, Dragon shouldn't cancel 180deg rotate animation
    // even if Player somehow teleported
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnRange")
    bool bOverrideStopMontageOnGoalReached = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnRange", meta=(EditCondition="bOverrideStopMontageOnGoalReached", EditConditionHides))
    bool bStopMontageOnGoalReached = false;
};

USTRUCT(Blueprintable)
struct FTurnRanges
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnRanges")
    TArray<FTurnRange> TurnRanges;
};


/**
 *
 */
// TODO make warning that not all ranges are covered
// TODO limit TurnAnimations count e.g. max 3 times, then give choice what to do - abort or success?
// TODO limit playing similar animations count e.g. max 3 times ?? and then give choice what to do - abort or success?
// TIPS
// - ranges can overlap, in such cases animation will be fired by order, so order is important
USTRUCT(Blueprintable)
struct FTurnSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TurnSettings", meta=(EditCondition="true", MultiLine))
    FString Notes = FString(TEXT(
                                 "Use cases:"
                                 "\n\n1) Big enemy (dragon, etc.) - better to use with \"bTurnOnlyWithAnims\", \"bStopMontageOnGoalReached\" and \"Precision = 1°\", BlendOut settings in AnimMontage becomes critcal for smooth visual ~0.5s blendout + inertionalization recommended"
                                 "\n\n2) Medium enemy (human size) - disable bTurnOnlyWithAnims nobody will mention that medium enemy rotates in place without anim"
                                 "\n\nTips: "
                                 "\n- ranges can overlap, in such cases animation will be fired by order, so order is important"
                                ));

    // if enabled - turn only with animations, if no ranges fits - consider it as success
    // if disabled - turn also with rotating enemy in place
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnSettings")
    bool bTurnOnlyWithAnims = true;
    // stops AnimMontage when reached goal, even if 180deg turn animation on half of playing - stop it
    // BlendOut settings for this option is critical
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnSettings")
    bool bStopMontageOnGoalReached = true;
    // TODO bChooseClosestInRaceCondition? если подходят 2 ренджа, в чью пользу принимать решение, зач если есть order TurnRange'ей

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnSettings", meta=(ForceInlineRow))
    TMap<FString, FTurnRanges> TurnRangesGroups;

    void Cleanup();
    void SetupPreset_Default_90_180();
    void SetupPreset_BigEnemy_90_180();
    void SetupPreset_45_90_180();
    void SetupPreset_15_45_90_180();
    void SetupPreset_15_30_45_90_180();
};


UCLASS(Blueprintable)
class UNREALHELPERLIBRARY_API UTurnSettingsDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnSettings")
    FTurnSettings TurnSettings;

    // Prefers to rotate 180deg if relative angle >115deg, suits for all medium mobs(human size)
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=1))
    void SetupPreset_Default_90_180();
    // All ranges have same proportion
    UFUNCTION(BlueprintCallable, Category="Setup", CallInEditor, meta=(EditCondition="bUseTurnAnimations && !bUseTurnSettingsDataAsset", EditConditionHides, DisplayPriority=1))
    void SetupPreset_BigEnemy_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=2))
    void SetupPreset_45_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=3))
    void SetupPreset_15_45_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=4))
    void SetupPreset_15_30_45_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=5))
    void Cleanup();
};
