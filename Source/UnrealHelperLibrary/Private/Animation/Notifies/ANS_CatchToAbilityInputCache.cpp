// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ANS_CatchToAbilityInputCache.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Core/UHLGameplayTags.h"


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
