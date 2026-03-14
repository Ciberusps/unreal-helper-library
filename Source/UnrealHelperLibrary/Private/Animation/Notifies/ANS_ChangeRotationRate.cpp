// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/ANS_ChangeRotationRate.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_ChangeRotationRate)

FString UANS_ChangeRotationRate::GetNotifyName_Implementation() const
{
	return FString::Printf(
		TEXT("Change RotationRate to Pitch=%.2f, Yaw=%.2f, Roll=%.2f"),
		RotationRate.Pitch,
		RotationRate.Yaw,
		RotationRate.Roll
	);
}

void UANS_ChangeRotationRate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	CharacterMovementComponent = Character->GetCharacterMovement();
	if (!CharacterMovementComponent.IsValid()) return;

	// UUHLCharacterMovementComponent* UHLCMC = GetUHLCharacterMovementComponent(Character);
	// if (!UHLCMC) return;

	// InitialRotationRate = UHLCMC->RotationRate; 
	// UHLCMC->SetRotationRate(RotationRate);

	InitialRotationRate = CharacterMovementComponent->RotationRate;
	CharacterMovementComponent->RotationRate = RotationRate;
}

void UANS_ChangeRotationRate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ReturnInitialRotationRate();

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_ChangeRotationRate::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (CharacterMovementComponent.IsValid())
	{
		ReturnInitialRotationRate();
	}
}

void UANS_ChangeRotationRate::ReturnInitialRotationRate()
{
	if (CharacterMovementComponent.IsValid())
	{
		CharacterMovementComponent->RotationRate = InitialRotationRate;
	}
}

// UUHLCharacterMovementComponent* UANS_ChangeRotationRate::GetUHLCharacterMovementComponent(ACharacter* Character)
// {
// 	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
// 	if (CharacterMovementComponent)
// 	{
// 		UUHLCharacterMovementComponent* UHLCharacterMovementComponent = Cast<UUHLCharacterMovementComponent>(CharacterMovementComponent);
// 		if (UHLCharacterMovementComponent)
// 		{
// 			return UHLCharacterMovementComponent;			
// 		}
// 	}
// 	return nullptr;
// }
