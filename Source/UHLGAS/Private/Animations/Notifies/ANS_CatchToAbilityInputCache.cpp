// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/ANS_CatchToAbilityInputCache.h"

#include "AbilitySystemInterface.h"
#include "UHLAbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/UHLGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_CatchToAbilityInputCache)

void UANS_CatchToAbilityInputCache::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    AActor* Actor = MeshComp->GetOwner();
    if (!IsValid(Actor)) return;

    checkf(Actor->Implements<UAbilitySystemInterface>(), TEXT("ANS_CatchToAbilityInputCache can be used only on characters nested from UHL AbilitySystem"));

    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor);
    if (AbilitySystemInterface == nullptr) return;

    ASC = Cast<UUHLAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
    checkf(ASC, TEXT("ANS_CatchToAbilityInputCache can be used only on characters nested from UHL AbilitySystem"));

    ASC->AddLooseGameplayTag(UHLGameplayTags::TAG_UHL_AbilityInputCache_Catching);
}

void UANS_CatchToAbilityInputCache::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!IsValid(ASC)) return;

    ASC->RemoveLooseGameplayTagCompletly(UHLGameplayTags::TAG_UHL_AbilityInputCache_Catching);
}
