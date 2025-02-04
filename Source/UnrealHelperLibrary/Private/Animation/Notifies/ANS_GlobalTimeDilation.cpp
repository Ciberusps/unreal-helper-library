// Copyright (c) 2024 Pavel Penkov


#include "Animation/Notifies/ANS_GlobalTimeDilation.h"

#include "Kismet/GameplayStatics.h"


void UANS_GlobalTimeDilation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	float GlobalTimeDilationCurveValue = 1.0f;
	MeshComp->GetAnimInstance()->GetCurveValueWithDefault("GlobalTimeDilation", 1.0f, GlobalTimeDilationCurveValue);

	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetOwner(), GlobalTimeDilationCurveValue);
}

void UANS_GlobalTimeDilation::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageBlendingOut(Montage, bInterrupted);

	UObject* Outer = Montage->GetOuter();
	UGameplayStatics::SetGlobalTimeDilation(Outer, 1.0f);
}

