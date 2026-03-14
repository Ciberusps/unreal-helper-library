// Pavel Penkov 2025 All Rights Reserved.

#include "Animation/Notifies/ANS_ChangeCharacterCapsule.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCapsuleComponent* UANS_ChangeCharacterCapsule::FindCapsuleComponent(USkeletalMeshComponent* MeshComp) const
{
    if (!MeshComp)
    {
        return nullptr;
    }

    AActor* OwningActor = MeshComp->GetOwner();
    if (!OwningActor)
    {
        return nullptr;
    }

    if (ACharacter* CharOwner = Cast<ACharacter>(OwningActor))
    {
        return CharOwner->GetCapsuleComponent();
    }

    return nullptr;
}
