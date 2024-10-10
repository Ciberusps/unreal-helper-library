// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/AN_UHL_Base.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AN_UHL_Base)

bool UAN_UHL_Base::IsPreviewWorld(USkeletalMeshComponent* MeshComp) const
{
    return MeshComp->GetWorld() && MeshComp->GetWorld()->IsPreviewWorld();
}

void UAN_UHL_Base::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}
