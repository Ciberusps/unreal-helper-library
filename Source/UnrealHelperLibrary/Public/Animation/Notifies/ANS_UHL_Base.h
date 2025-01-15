// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UHL_Base.generated.h"

class USkeletalMeshComponent;

/**
 * with events like OnMontageBlendOut, OnMontageInterrupted...
 * you can disable subscribing OnMontageBlendOut, by "bUseOnMontageBlendingOut=false" in constructor
 */
UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UANS_UHL_Base : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ANS_UHL_Base")
	bool bUseOnMontageBlendingOut = true;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

/** Experimental **/
	// Should use experimental features like "NotifyEndOrBlendOut"
	virtual bool ShouldUseExperimentalUHLFeatures() const { return false; };
	// experimental works only with "ShouldUseExperimentalUHLFeatures" enabled
	virtual void NotifyEndOrBlendOut(USkeletalMeshComponent* MeshComp);
/** ~Experimental **/

	UFUNCTION()
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	// TODO OnMontageBlendOut, OnMontageInterrupted, OnCancelled ...
};
