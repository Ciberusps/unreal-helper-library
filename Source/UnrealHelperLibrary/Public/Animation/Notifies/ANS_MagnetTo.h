// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BehaviorTree/BlackboardData.h"
#include "ANS_MagnetTo.generated.h"

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_MagnetTo : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    // UPROPERTY(Category=Decorator, EditAnywhere)
    // bool bUseBlackboardActor = false;
    UPROPERTY(Category="Decorator", EditAnywhere)
    bool bMagnetToActorFromBlackboard = false;
    UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="bMagnetToActorFromBlackboard", EditConditionHides))
    FBlackboardKeySelector BlackboardActor;

    UPROPERTY(Category="Decorator", EditAnywhere, meta=(Units = "Centimeters"))
    float Distance = 200.0f;
    UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditCondition="!bMagnetToActorFromBlackboard", EditConditionHides))
    float Angle = 0.0f;

    UPROPERTY(Category="Decorator", EditAnywhere)
    bool bCustomTime = false;

#if WITH_EDITOR
    /** Override this to prevent firing this notify state type in animation editors */
    virtual bool ShouldFireInEditor() { return false; }
#endif

    virtual FLinearColor GetEditorColor() override { return FLinearColor(0.0f, 0.74f, 1.0f, 1.0f); };
    virtual FString GetNotifyName_Implementation() const override { return FString("MagnetTo"); };

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
    TWeakObjectPtr<ACharacter> BaseCharacter;
    EMovementMode InitialMovementMode;
    float Speed = 0.0f;
    float Alpha = 0.0f;

    UFUNCTION()
    void TimerTick();

    // UFUNCTION()
    // void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
};
