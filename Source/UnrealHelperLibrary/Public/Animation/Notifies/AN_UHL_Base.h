// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_UHL_Base.generated.h"

/**
 *
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifySignature, USkeletalMeshComponent*)

UCLASS()
class UNREALHELPERLIBRARY_API UAN_UHL_Base : public UAnimNotify
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool IsPreviewWorld(USkeletalMeshComponent* MeshComp) const;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
    FOnNotifySignature OnNotified;
};
