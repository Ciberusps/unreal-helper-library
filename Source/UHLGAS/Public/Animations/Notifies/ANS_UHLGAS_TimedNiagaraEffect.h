// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "GameplayTagContainer.h"
#include "ANS_UHLGAS_TimedNiagaraEffect.generated.h"

/**
 * Drop in replacement for "AnimNotifyState_TimedNiagaraEffect" but with GAS functions
 * like checking that tags are present on Actor
 */
UCLASS()
class UHLGAS_API UANS_UHLGAS_TimedNiagaraEffect : public UAnimNotifyState_TimedNiagaraEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara System")
	FGameplayTagContainer ActivationRequiredTags = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara System")
	FGameplayTagContainer ActivationBlockedTags = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara System")
	bool bIgnoreTagRequirementsInPreview = true;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	bool GetAbilitySystemChecksRequired() const;

	bool CanActivateNiagara(AActor* Owner) const;
};
