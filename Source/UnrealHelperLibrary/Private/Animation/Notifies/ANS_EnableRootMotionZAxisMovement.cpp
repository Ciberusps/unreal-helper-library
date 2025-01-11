// Copyright (c) 2024 Pavel Penkov


#include "Animation/Notifies/ANS_EnableRootMotionZAxisMovement.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utils/UHLTraceUtilsBPL.h"
#include "Utils/UnrealHelperLibraryBPL.h"
#include "DrawDebugHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_EnableRootMotionZAxisMovement)

UANS_EnableRootMotionZAxisMovement::UANS_EnableRootMotionZAxisMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LandCheckBlendOutSettings = FMontageBlendSettings();
	LandCheckBlendOutSettings.Blend = 1.0f;
	LandCheckBlendOutSettings.BlendMode = EMontageBlendMode::Inertialization;
}

void UANS_EnableRootMotionZAxisMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    ACharacter* BaseCharacter = Cast<ACharacter>(MeshComp->GetOwner());
    if (!BaseCharacter) return;

    UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
    InitialMovementMode = MovementComponent->MovementMode;
    MovementComponent->SetMovementMode(MOVE_Flying);
}

void UANS_EnableRootMotionZAxisMovement::NotifyEndOrBlendOut(USkeletalMeshComponent* MeshComp)
{
	Super::NotifyEndOrBlendOut(MeshComp);

	if (!MeshComp) return;

	ACharacter* BaseCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (!BaseCharacter) return;
	
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
	if (bStopMontageIfLandCheckFails)
	{
		FVector CurrentLocation = BaseCharacter->GetActorLocation();
		FVector EndLocation = CurrentLocation + FVector(0.0f, 0.0f, -LandCheckDistance);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(BaseCharacter);
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
			FAnimMontageInstance* AnimMontage = BaseCharacter->GetRootMotionAnimMontageInstance();
			if (AnimMontage && AnimMontage->IsValid())
			{
				AnimMontage->PushDisableRootMotion();
				AnimMontage->Stop(LandCheckBlendOutSettings, false);
			}
			else
			{
				UUnrealHelperLibraryBPL::DebugPrintString(
					BaseCharacter->GetWorld(),
					FString::Printf(TEXT("UANS_EnableRootMotionZAxisMovement::NotifyEndOrBlendOut on %s error root motion AnimMontage not found"), *BaseCharacter->GetName())
				);
			}
		}
		else
		{
			if (HitResult.IsValidBlockingHit() && bDebug)
			{
				DrawDebugString(
            		BaseCharacter->GetWorld(), HitResult.Location, FString::Printf(TEXT("Land check: %s"),
            		*HitResult.GetActor()->GetName()), nullptr,
            		FColor::Green, 5.0f, true, 1.0f 
            	);
			}
		}
	}
}
