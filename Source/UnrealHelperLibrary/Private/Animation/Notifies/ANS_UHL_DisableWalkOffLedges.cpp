// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/ANS_UHL_DisableWalkOffLedges.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_UHL_DisableWalkOffLedges::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;

    ACharacter* OwnerChar = Cast<ACharacter>(MeshComp->GetOwner());
    if (!OwnerChar) return;

    UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement();
    if (!MoveComp) return;

    // Store original
    bOriginalCanWalkOffLedges = MoveComp->bCanWalkOffLedges;
    OriginalPerchRadius = MoveComp->PerchRadiusThreshold;

    // Apply new settings
    MoveComp->bCanWalkOffLedges = false;
    if (PerchRadiusThreshold >= 0.0f)
    {
        MoveComp->PerchRadiusThreshold = PerchRadiusThreshold;
    }
    else
    {
        MoveComp->PerchRadiusThreshold = OwnerChar->GetCapsuleComponent()->GetScaledCapsuleRadius();
    }
}

void UANS_UHL_DisableWalkOffLedges::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;

    ACharacter* OwnerChar = Cast<ACharacter>(MeshComp->GetOwner());
    if (!OwnerChar) return;

    UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement();
    if (!MoveComp) return;

    // Restore original if we stored them
    MoveComp->bCanWalkOffLedges = bOriginalCanWalkOffLedges;
    MoveComp->PerchRadiusThreshold = OriginalPerchRadius;
}
