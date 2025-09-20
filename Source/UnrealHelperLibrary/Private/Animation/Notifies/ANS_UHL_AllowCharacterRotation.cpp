// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ANS_UHL_AllowCharacterRotation.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


FString UANS_UHL_AllowCharacterRotation::GetNotifyName_Implementation() const
{
    // Base name
    FString NotifyName = TEXT("AllowCharacterRotation");

    // Optional rotation‐rate suffix
    if (bChangeRotationRate)
    {
        NotifyName += FString::Printf(
            TEXT(" + change RotationRate to Pitch=%.2f, Yaw=%.2f, Roll=%.2f"),
            RotationRate.Pitch,
            RotationRate.Yaw,
            RotationRate.Roll
        );
    }

    return NotifyName;
}

void UANS_UHL_AllowCharacterRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	CharacterMovementComponent = Character->GetCharacterMovement();
	if (!CharacterMovementComponent.IsValid()) return;

    CharacterMovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = true;

    if (bChangeRotationRate)
    {
        InitialRotationRate = CharacterMovementComponent->RotationRate;
        CharacterMovementComponent->RotationRate = RotationRate;
    }
}

void UANS_UHL_AllowCharacterRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ReturnDefaults();

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_UHL_AllowCharacterRotation::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	ReturnDefaults();
}

void UANS_UHL_AllowCharacterRotation::ReturnDefaults()
{
	if (CharacterMovementComponent.IsValid())
	{
        CharacterMovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = false;
	    if (bChangeRotationRate)
	    {
            CharacterMovementComponent->RotationRate = InitialRotationRate;
	    }
	}
}
