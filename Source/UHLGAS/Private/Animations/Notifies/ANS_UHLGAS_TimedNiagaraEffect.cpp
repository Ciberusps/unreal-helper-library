// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/ANS_UHLGAS_TimedNiagaraEffect.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UANS_UHLGAS_TimedNiagaraEffect::NotifyBegin(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	if (CanActivateNiagara(Owner))
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	}
}

bool UANS_UHLGAS_TimedNiagaraEffect::GetAbilitySystemChecksRequired() const
{
	return  ActivationRequiredTags.Num() > 0
		|| ActivationBlockedTags.Num() > 0;
}

bool UANS_UHLGAS_TimedNiagaraEffect::CanActivateNiagara(AActor* Owner) const
{
	if (bIgnoreTagRequirementsInPreview && Owner->GetWorld()->IsPreviewWorld()) return true;
	if (!GetAbilitySystemChecksRequired()) return true;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC) return false;
		
	const FGameplayTagContainer OwnedTags = ASC->GetOwnedGameplayTags();
	if (OwnedTags.HasAll(ActivationRequiredTags) && !OwnedTags.HasAny(ActivationBlockedTags))
	{
		return true;
	}

	return false;
}