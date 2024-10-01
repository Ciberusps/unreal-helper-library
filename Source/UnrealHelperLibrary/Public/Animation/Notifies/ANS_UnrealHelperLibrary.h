// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UnrealHelperLibrary.generated.h"

/**
 * with events like OnMontageBlendOut, OnMontageInterrupted...
 * you can disable subscribing OnMontageBlendOut, by "bUseOnMontageBlendingOut=false" in constructor
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_UnrealHelperLibrary : public UAnimNotifyState
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bUseOnMontageBlendingOut = true;
	TWeakObjectPtr<const UAnimMontage> CurrentAnimMontage;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	// TODO OnMontageBlendOut, OnMontageInterrupted, OnCancelled ...
};
