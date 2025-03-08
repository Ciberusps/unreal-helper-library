// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/ANS_CheckAbilityInputCache.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_CheckAbilityInputCache)

void UANS_CheckAbilityInputCache::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    AActor* Actor = MeshComp->GetOwner();
    if (!IsValid(Actor)) return;

    checkf(Actor->Implements<UAbilitySystemInterface>(), TEXT("ANS_CheckAbilityInputCache can be used only on characters nested from UHL AbilitySystem"));

    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor);
    if (AbilitySystemInterface == nullptr) return;

    ASC = Cast<UUHLAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
    checkf(ASC, TEXT("ANS_CheckAbilityInputCache can be used only on characters nested from UHL AbilitySystem"));
}

void UANS_CheckAbilityInputCache::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    if (IsValid(ASC))
    {
		ASC->GetAbilityInputCache()->CheckCache();
    }
}

void UANS_CheckAbilityInputCache::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (bClearCacheOnEnd && IsValid(ASC))
    {
        ASC->GetAbilityInputCache()->ClearCache();
    }
}
