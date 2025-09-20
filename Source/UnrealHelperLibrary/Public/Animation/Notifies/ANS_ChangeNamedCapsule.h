// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ANS_ChangeCapsuleBase.h"
#include "ANS_ChangeNamedCapsule.generated.h"

/**
 * ANS that looks for a UCapsuleComponent by its component‐name on the actor.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Change Named Capsule"))
class UNREALHELPERLIBRARY_API UANS_ChangeNamedCapsule : public UANS_ChangeCapsuleBase
{
    GENERATED_BODY()

public:
    /** Exact component‐name (FName) of the UCapsuleComponent to modify (e.g. "CapsuleVisual"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule|Lookup")
    FName CapsuleComponentName = NAME_None;

protected:
    /** Return the capsule whose Component Name matches. */
    virtual UCapsuleComponent* FindCapsuleComponent(USkeletalMeshComponent* MeshComp) const override;
};
