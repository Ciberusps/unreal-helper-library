// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ANS_EnableRootMotionZAxisMovement.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_EnableRootMotionZAxisMovement)

void UANS_EnableRootMotionZAxisMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    BaseCharacter = Cast<ACharacter>(MeshComp->GetOwner());
    if (!BaseCharacter.IsValid()) return;

    UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
    InitialMovementMode = MovementComponent->MovementMode;
    MovementComponent->SetMovementMode(MOVE_Flying);

    BaseCharacter->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddUniqueDynamic(this, &UANS_EnableRootMotionZAxisMovement::OnMontageBlendingOut);
}

void UANS_EnableRootMotionZAxisMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!BaseCharacter.IsValid()) return;

    BaseCharacter->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.RemoveDynamic(this, &UANS_EnableRootMotionZAxisMovement::OnMontageBlendingOut);
}

void UANS_EnableRootMotionZAxisMovement::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    if (!BaseCharacter.IsValid()) return;

    UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
    if (MovementComponent->MovementMode == MOVE_Flying)
    {
        MovementComponent->SetMovementMode(MOVE_Falling);
    }
}
