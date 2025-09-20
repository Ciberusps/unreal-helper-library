// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/ANS_UHL_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ANS_UHL_AllowCharacterRotation.generated.h"

/**
 * CharacterMovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = true
 * and opportunity to change RotationRate
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_UHL_AllowCharacterRotation : public UANS_UHL_Base
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
    bool bChangeRotationRate = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility", meta=(EditCondition="bChangeRotationRate"))
    FRotator RotationRate = FRotator(0.0f, 0.0f, 90.0f);

protected:
#if WITH_EDITOR
    /** Override this to prevent firing this notify state type in animation editors */
    virtual bool ShouldFireInEditor() { return false; }
#endif

    virtual FLinearColor GetEditorColor() override { return FLinearColor(0.799103f, 0.254152f, 0.730461f); };
    virtual FString GetNotifyName_Implementation() const override;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

    virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) override;

private:
    UPROPERTY()
    FRotator InitialRotationRate = FRotator::ZeroRotator;

    TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

    UFUNCTION()
    void ReturnDefaults();
};
