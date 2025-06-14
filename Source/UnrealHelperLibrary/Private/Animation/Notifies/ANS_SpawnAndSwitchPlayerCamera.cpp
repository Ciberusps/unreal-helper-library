// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/ANS_SpawnAndSwitchPlayerCamera.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UANS_SpawnAndSwitchPlayerCamera::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (!MeshComp || !CameraToSpawnClass)
    {
        return;
    }

    // 1. Get the owning actor and its PlayerController
    AActor* OwningActor = MeshComp->GetOwner();
    if (!OwningActor)
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(OwningActor->GetInstigatorController());
    if (!PC)
    {
        // If the owning actor isn’t possessed by a PC, try another route
        PC = UGameplayStatics::GetPlayerController(OwningActor->GetWorld(), PlayerControllerIndex);
    }
    if (!PC)
    {
        return;
    }

    // 2. Store the previous view target (so we can return to it later)
    PreviousViewTarget = PC->GetViewTarget();

    // 3. Spawn the camera actor at the owner’s location (or at a specific socket)
    FTransform SpawnTransform = OwningActor->GetActorTransform();
    if (AttachSocketName != NAME_None)
    {
        // If we want to attach to a socket/bone, get its world transform
        if (MeshComp->DoesSocketExist(AttachSocketName))
        {
            SpawnTransform = MeshComp->GetSocketTransform(AttachSocketName);
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = OwningActor;
    SpawnParams.Instigator = Cast<APawn>(OwningActor);

    SpawnedCameraInstance = Cast<ACameraActor>(
        OwningActor->GetWorld()->SpawnActor<ACameraActor>(
            CameraToSpawnClass,
            SpawnTransform,
            SpawnParams
        )
    );

    if (!SpawnedCameraInstance)
    {
        return;
    }

    // Optionally, attach the spawned camera to the owner (so it follows)
    if (AttachSocketName != NAME_None)
    {
        SpawnedCameraInstance->AttachToComponent(
            MeshComp,
            AttachRules.ToEngineRules(),
            AttachSocketName
        );
    }

    // 4. Blend to the spawned camera
	PC->SetViewTargetWithBlend(SpawnedCameraInstance, BlendInTime, BlendInFunction);
}

void UANS_SpawnAndSwitchPlayerCamera::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!MeshComp)
    {
        return;
    }

    // 1. Try to get the same PlayerController we used before
    AActor* OwningActor = MeshComp->GetOwner();
    if (!OwningActor)
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(OwningActor->GetInstigatorController());
    if (!PC)
    {
        PC = UGameplayStatics::GetPlayerController(OwningActor->GetWorld(), 0);
    }
    if (!PC)
    {
        return;
    }

    // 2. Blend back to the original view target if it’s still valid
    if (PreviousViewTarget)
    {
        PC->SetViewTargetWithBlend(PreviousViewTarget, BlendOutTime, BlendOutFunction);
    }

    // 3. Destroy the spawned camera actor
    if (SpawnedCameraInstance)
    {
        SpawnedCameraInstance->SetLifeSpan(3);
        SpawnedCameraInstance = nullptr;
    }

    // 4. Clear the stored pointer
    PreviousViewTarget = nullptr;
}
