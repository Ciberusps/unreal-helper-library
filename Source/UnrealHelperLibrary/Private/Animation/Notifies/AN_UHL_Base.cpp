// Copyright (c) 2024 Pavel Penkov


#include "Animation/Notifies/AN_UHL_Base.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AN_UHL_Base)

void UAN_UHL_Base::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}
