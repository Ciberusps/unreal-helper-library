// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ANS_UHL_Base.h"
#include "GameplayTagContainer.h"
#include "ANS_ActivateAbility.generated.h"

class IAbilitySystemInterface;

/**
 *
 */
UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UANS_ActivateAbility : public UANS_UHL_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	FGameplayTag GameplayAbilityTag = FGameplayTag::EmptyTag;
	// TODO check should work only if montage exists
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	bool bDeactivateOnMontageBlendingOut = true;
	// by default don't allow remote execution, cause its requires time
	// abilities in ANS's mostly don't have that time
	// and not impacts gameplay, only enhances input and game feel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	bool bAllowRemoteActivation = false;

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
#endif
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) override;

private:
	TWeakInterfacePtr<IAbilitySystemInterface> ActorWithASC;

	UFUNCTION()
	void CancelAbility();
};
