// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/ANS_MagnetTo.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_MagnetTo)

void UANS_MagnetTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    // Speed = Distance / TotalDuration / 60;
    BaseCharacter = Cast<ACharacter>(MeshComp->GetOwner());

    // FTimerHandle TimerHandle;
    // MeshComp->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UANS_MagnetTo::TimerTick, 0.0f, true, -1);

    // GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseDestructibleActor::FinalDestruct, TimeToDisappear);
}

void UANS_MagnetTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    Alpha = FMath::Clamp(Alpha + FrameDeltaTime, 0, 1);
    FVector NewLocation = BaseCharacter->GetActorLocation();
    float Delta = UKismetMathLibrary::Ease(0, Distance / 1000, Alpha, EEasingFunc::Linear);
    NewLocation += BaseCharacter->GetActorForwardVector() * Delta;
    BaseCharacter->SetActorLocation(NewLocation, true);
    // BaseCharacter.Get()->GetCharacterMovement()->AddForce(BaseCharacter-> GetActorForwardVector() * Distance);
}

void UANS_MagnetTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);
}
