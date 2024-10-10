// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_InterpolateToPosition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterpolateToPositionSimpleDelegate);

/**
 * Based on "AbilityTask_MoveToLocation"
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UAT_InterpolateToPosition : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_InterpolateToPosition(const FObjectInitializer& ObjectInitializer);

	/** Move to the specified location, using the vector curve (range 0 - 1) if specified, otherwise the float curve (range 0 - 1) or fallback to linear interpolation */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_InterpolateToPosition* InterpolateToPosition(
	    UGameplayAbility* OwningAbility,
	    FName TaskInstanceName,
	    FVector Location,
	    FRotator Rotation,
	    float Duration,
	    UCurveFloat* OptionalInterpolationCurve,
	    UCurveVector* OptionalVectorInterpolationCurve,
	    AActor* OptionalActorToInterpolate,
	    const bool bIsIgnoreHits = true,
	    const float DistanceToHit = 0.f
	);

	UPROPERTY(BlueprintAssignable)
	FInterpolateToPositionSimpleDelegate OnTargetLocationReached;

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void PreDestroyFromReplication() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	bool bIsFinished;

	UPROPERTY(Replicated)
	FRotator StartRotation;
	UPROPERTY(Replicated)
	FRotator TargetRotation;

	UPROPERTY(Replicated)
	FVector StartLocation;
	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	float DurationOfMovement;

	float TimeMoveStarted;
	float TimeMoveWillEnd;

	UPROPERTY(Replicated)
	TObjectPtr<UCurveFloat> LerpCurve;
	UPROPERTY(Replicated)
	TObjectPtr<UCurveVector> LerpCurveVector;
    UPROPERTY(Replicated)
    TObjectPtr<AActor> ActorToInterpolate = nullptr;

    static bool CheckHit(
        const UWorld* World,
        const FVector& Start,
        const FVector& End,
        const TArray<AActor*>& ActorsToIgnore,
        FHitResult& OutHit
    );
    static FVector GetCurrentEndLocation(const FVector& Start, const FVector& End, const float DistanceToHit = 0.f);
    static float GetCurrentDuration(const float Duration, const FVector& Start, const FVector& End, const FVector& CurrentEnd);
};
