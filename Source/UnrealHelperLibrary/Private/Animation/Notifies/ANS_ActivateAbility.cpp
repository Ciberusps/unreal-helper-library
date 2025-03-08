// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/ANS_ActivateAbility.h"

#include "Engine/World.h"
#include "Animation/AnimMontage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_ActivateAbility)

FString UANS_ActivateAbility::GetNotifyName_Implementation() const
{
	return FString("ActivateAbility ") + GameplayAbilityTag.ToString();
}

void UANS_ActivateAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!GameplayAbilityTag.IsValid())
	{
		UUnrealHelperLibraryBPL::DebugPrintString(MeshComp->GetWorld(), FString::Printf(TEXT("%s GameplayAbilityTag not set"), *this->GetName()));
		return;
	}
	
	ActorWithASC = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	if (ActorWithASC.IsValid())
	{
		ActorWithASC->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayAbilityTag), bAllowRemoteActivation);
	}
}

void UANS_ActivateAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!GameplayAbilityTag.IsValid())
	{
		UUnrealHelperLibraryBPL::DebugPrintString(MeshComp->GetWorld(), FString::Printf(TEXT("%s GameplayAbilityTag not set"), *this->GetName()));
		return;
	}

	CancelAbility();
}

void UANS_ActivateAbility::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bDeactivateOnMontageBlendingOut || !Montage)
	{
		return;
	}
		
	if (ActorWithASC.IsValid())
	{
		CancelAbility();
	}
}

void UANS_ActivateAbility::CancelAbility()
{
	if (ActorWithASC.IsValid())
	{
		const FGameplayTagContainer Tags = FGameplayTagContainer(GameplayAbilityTag);
		ActorWithASC->GetAbilitySystemComponent()->CancelAbilities(&Tags);
	}
}
