// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/ANS_UHLGAS_GlobalTimeDilation.h"

#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"


void UANS_UHLGAS_GlobalTimeDilation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	float GlobalTimeDilationCurveValue = 1.0f;
	MeshComp->GetAnimInstance()->GetCurveValueWithDefault("GlobalTimeDilation", 1.0f, GlobalTimeDilationCurveValue);

	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetOwner(), GlobalTimeDilationCurveValue);
}

void UANS_UHLGAS_GlobalTimeDilation::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageBlendingOut(Montage, bInterrupted);

	UObject* Outer = Montage->GetOuter();
	UGameplayStatics::SetGlobalTimeDilation(Outer, 1.0f);
}

