// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ANS_UnrealHelperLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_UnrealHelperLibrary)

void UANS_UnrealHelperLibrary::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CurrentAnimMontage = EventReference.GetNotify()->GetLinkedMontage();

	if (CurrentAnimMontage.IsValid())
	{
		if (bUseOnMontageBlendingOut)
		{
			MeshComp->AnimScriptInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &UANS_UnrealHelperLibrary::OnMontageBlendingOut);
		}
	}
}

void UANS_UnrealHelperLibrary::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (bUseOnMontageBlendingOut)
	{
		MeshComp->AnimScriptInstance->OnMontageBlendingOut.RemoveDynamic(this, &UANS_UnrealHelperLibrary::OnMontageBlendingOut);
	}
}

void UANS_UnrealHelperLibrary::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!CurrentAnimMontage.IsValid()
		|| Montage != CurrentAnimMontage)
	{
		return;
	}
}