// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnSettings.generated.h"


USTRUCT(Blueprintable)
struct FTurnRange
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FFloatRange Range = FFloatRange(-135, -45);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AnimMontage = nullptr;
};

USTRUCT(Blueprintable)
struct FTurnRanges
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FTurnRange> TurnRanges;
};


/**
 *
 */
// TODO make warning that not all ranges are covered
// TIPS
// - ranges can overlap, in such cases animation will be fired by order, so order is important
USTRUCT(Blueprintable)
struct FTurnSettings
{
    GENERATED_BODY()

public:
    // turn only with animations if no ranges fits - consider it as success
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTurnOnlyWithAnims = true;
    // stops AnimMontage when reached goal, even if 180deg turn in half of animation stop it
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bStopMontageOnGoalReached = true;
    // TODO bChooseClosestInRaceCondition? если подходят 2 ренджа, в чью пользу принимать решение

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceInlineRow))
    TMap<FString, FTurnRanges> TurnRangesGroups;

    void Cleanup();
    void SetupPreset_90_180();
    void SetupPreset_45_90_180();
    void SetupPreset_15_45_90_180();
    void SetupPreset_15_30_45_90_180();
};


UCLASS(Blueprintable)
class UNREALHELPERLIBRARY_API UTurnToAnimationsDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTurnSettings TurnSettings;

    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=1))
    void SetupPreset_Recommended_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=2))
    void SetupPreset_45_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=3))
    void SetupPreset_15_45_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=4))
    void SetupPreset_15_30_45_90_180();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Setup", meta = (DisplayPriority=5))
    void Cleanup();
};
