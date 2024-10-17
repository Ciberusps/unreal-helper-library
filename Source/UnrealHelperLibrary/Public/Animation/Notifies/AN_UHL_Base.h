// Copyright (c) 2024 Pavel Penkov

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
    // TODO remove, deprecated use UUnrealHelperLibraryBPL::IsObjectInPreviewWorld
    UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction, DeprecationMessage="use !UUnrealHelperLibraryBPL::IsObjectInGameWorld"))
    bool IsPreviewWorld(USkeletalMeshComponent* MeshComp) const;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
    FOnNotifySignature OnNotified;
};
