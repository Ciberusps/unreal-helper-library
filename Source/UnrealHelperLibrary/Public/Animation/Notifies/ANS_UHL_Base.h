// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UHL_Base.generated.h"

/**
 * with events like OnMontageBlendOut, OnMontageInterrupted...
 * you can disable subscribing OnMontageBlendOut, by "bUseOnMontageBlendingOut=false" in constructor
 */
UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UANS_UHL_Base : public UAnimNotifyState
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ANS_UHL_Base")
	bool bUseOnMontageBlendingOut = true;

    TWeakObjectPtr<const UAnimMontage> CurrentAnimMontage;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	// TODO OnMontageBlendOut, OnMontageInterrupted, OnCancelled ...
};
