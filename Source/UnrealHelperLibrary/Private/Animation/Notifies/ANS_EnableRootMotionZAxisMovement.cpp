// Copyright (c) 2024 Pavel Penkov


#include "Animation/Notifies/ANS_EnableRootMotionZAxisMovement.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_EnableRootMotionZAxisMovement)

void UANS_EnableRootMotionZAxisMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    BaseCharacter = Cast<ACharacter>(MeshComp->GetOwner());
    if (!BaseCharacter.IsValid()) return;

    UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
    InitialMovementMode = MovementComponent->MovementMode;
    MovementComponent->SetMovementMode(MOVE_Flying);
}

void UANS_EnableRootMotionZAxisMovement::NotifyEndOrBlendOut(USkeletalMeshComponent* MeshComp)
{
	Super::NotifyEndOrBlendOut(MeshComp);

	UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
	if (MovementComponent->MovementMode == MOVE_Flying)
	{
		MovementComponent->SetMovementMode(MOVE_Falling);
	}
}
