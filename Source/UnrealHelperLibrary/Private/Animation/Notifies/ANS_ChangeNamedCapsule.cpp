// Pavel Penkov 2025 All Rights Reserved.

#include "Animation/Notifies/ANS_ChangeNamedCapsule.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"

UCapsuleComponent* UANS_ChangeNamedCapsule::FindCapsuleComponent(USkeletalMeshComponent* MeshComp) const
{
    if (!MeshComp)
    {
        return nullptr;
    }

    AActor* OwningActor = MeshComp->GetOwner();
    if (!OwningActor || CapsuleComponentName.IsNone())
    {
        return nullptr;
    }

    TArray<UCapsuleComponent*> FoundComps;
    OwningActor->GetComponents<UCapsuleComponent>(FoundComps);

    for (UCapsuleComponent* Cap : FoundComps)
    {
        if (Cap && Cap->GetFName() == CapsuleComponentName)
        {
            return Cap;
        }
    }

    return nullptr;
}
