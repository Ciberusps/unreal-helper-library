// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/ANS_UHL_Base.h"
#include "ANS_UHL_DisableWalkOffLedges.generated.h"

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_UHL_DisableWalkOffLedges : public UANS_UHL_Base
{
	GENERATED_BODY()

public:
    /** By default half capsule radius will be used
     * but If >= 0, will override the movement component's PerchRadiusThreshold. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge Settings")
    float PerchRadiusThreshold = -1.0f;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
    // To restore after end
    bool bOriginalCanWalkOffLedges;
    float OriginalPerchRadius;
};
