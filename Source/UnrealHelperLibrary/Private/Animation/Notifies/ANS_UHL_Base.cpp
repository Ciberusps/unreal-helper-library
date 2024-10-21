// Copyright (c) 2024 Pavel Penkov


#include "Animation/Notifies/ANS_UHL_Base.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_UHL_Base)

void UANS_UHL_Base::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CurrentAnimMontage = EventReference.GetNotify()->GetLinkedMontage();

	if (CurrentAnimMontage.IsValid())
	{
		if (bUseOnMontageBlendingOut)
		{
			MeshComp->AnimScriptInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &UANS_UHL_Base::OnMontageBlendingOut);
		}
	}
}

void UANS_UHL_Base::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (bUseOnMontageBlendingOut)
	{
		MeshComp->AnimScriptInstance->OnMontageBlendingOut.RemoveDynamic(this, &UANS_UHL_Base::OnMontageBlendingOut);
	}
}

void UANS_UHL_Base::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!CurrentAnimMontage.IsValid()
		|| Montage != CurrentAnimMontage)
	{
		return;
	}
}
