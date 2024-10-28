// Copyright (c) 2024 Pavel Penkov


#include "Animation/Notifies/ANS_EnableRootMotionZAxisMovement.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utils/UHLTraceUtilsBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_EnableRootMotionZAxisMovement)

void UANS_EnableRootMotionZAxisMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    BaseCharacter = Cast<ACharacter>(MeshComp->GetOwner());
    if (!BaseCharacter.IsValid()) return;

    UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
    InitialMovementMode = MovementComponent->MovementMode;
    MovementComponent->SetMovementMode(MOVE_Flying);

	BaseCharacter->LandedDelegate.AddUniqueDynamic(this, &UANS_EnableRootMotionZAxisMovement::OnLanded);
}

void UANS_EnableRootMotionZAxisMovement::NotifyEndOrBlendOut(USkeletalMeshComponent* MeshComp)
{
	Super::NotifyEndOrBlendOut(MeshComp);
	
	UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
	if (MovementComponent->MovementMode == MOVE_Flying)
	{
		MovementComponent->SetMovementMode(MOVE_Falling);
	}

	// 1) land до окончания, когда например запрыгиваем куда-то -
	// а OnLanded в этом случае вообще будет работать, кмк - нет.
	// Значит нужно чекать самостоятельно "land" и на MOVE_Falling переключать?
	// UPD вообще не нужно ничо чекать, ANS должен закончится до/вовремя Land'а
	// а значит никаких проблем с этим нет
	// 2) ANS кончился а мы при этом не приземлились на землю и до земли далеко
	// нужно Stop'ить montage, а для этого сделать sweep test капсулы вниз
	if (bMakeLandCheckOnEnd)
	{
		FVector CurrentLocation = BaseCharacter->GetActorLocation();
		FVector EndLocation = CurrentLocation + FVector(0, 0, LandCheckDistance);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(BaseCharacter.Get());
		// ignore all attached actors
		TArray<AActor*> CharacterAttachedActors;
		BaseCharacter->GetAttachedActors(CharacterAttachedActors, false, true);

		FHitResult HitResult;
		// TODO use Sphere instead of capsule
		bool bHasHit = UUHLTraceUtilsBPL::SweepCapsuleSingleByChannel(
			BaseCharacter->GetWorld(),
			HitResult,
			CurrentLocation,
			EndLocation,
			BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius(),
			BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
			BaseCharacter->GetActorRotation().Quaternion(),
			CollisionChannel,
			CollisionParams,
			FCollisionResponseParams::DefaultResponseParam,
			bDebug,
			5.0f,
			FColor::Red,
			FColor::Yellow,
			0.2f
		);

		if (!bHasHit)
		{
			// TODO try stop specific montage
			// const UAnimMontage* AnimMontage = CurrentAnimMontage.Get();
			// BaseCharacter->StopAnimMontage();
			FMontageBlendSettings BlendSettings;
			BlendSettings.Blend = 0.25f;
			BlendSettings.BlendMode = EMontageBlendMode::Inertialization;
			BaseCharacter.Get()->GetRootMotionAnimMontageInstance()->PushDisableRootMotion();
			BaseCharacter.Get()->GetRootMotionAnimMontageInstance()->Stop(BlendSettings, true);
		}
		else
		{
			DrawDebugString(
				BaseCharacter->GetWorld(), HitResult.Location, FString::Printf(TEXT("Land check: %s"),
				*HitResult.GetActor()->GetName()), nullptr,
				FColor::Green, 5.0f, true, 2.0f 
			);
		}
	}
}

void UANS_EnableRootMotionZAxisMovement::OnLanded(const FHitResult& HitResult)
{
	int32 test = 0;
}
