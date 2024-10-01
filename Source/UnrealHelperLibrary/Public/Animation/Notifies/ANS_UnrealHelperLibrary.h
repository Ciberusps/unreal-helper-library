// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UnrealHelperLibrary.generated.h"

/**
 * with events like OnMontageBlendOut, OnMontageInterrupted...
 * you can disable subscribing OnMontageBlendOut, by "bUseOnMontageBlendOut=false" in constructor
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_UnrealHelperLibrary : public UAnimNotifyState
{
	GENERATED_BODY()

public:

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// TODO OnMontageBlendOut, OnMontageInterrupted, ....
};
