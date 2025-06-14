// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ANS_ChangeCapsuleBase.h"
#include "ANS_ChangeCharacterCapsule.generated.h"

/**
 * ANS that always targets the owning ACharacter’s capsule component.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Change Character Capsule"))
class UNREALHELPERLIBRARY_API UANS_ChangeCharacterCapsule : public UANS_ChangeCapsuleBase
{
    GENERATED_BODY()

protected:
    /** Return the Character’s UCapsuleComponent, or nullptr if none. */
    virtual UCapsuleComponent* FindCapsuleComponent(USkeletalMeshComponent* MeshComp) const override;
};
