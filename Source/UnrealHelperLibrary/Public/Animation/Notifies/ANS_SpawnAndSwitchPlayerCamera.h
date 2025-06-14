// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/ANS_UHL_Base.h"
#include "Camera/CameraActor.h"
#include "Core/UHLAttachmentRules.h"
#include "ANS_SpawnAndSwitchPlayerCamera.generated.h"

// TODO: probably offset property required

/**
 * When this AnimNotifyState begins, it spawns a camera (CameraToSpawnClass),
 * blends the view to it over BlendInTime, and when the notify ends,
 * blends back to the original view target over BlendTimeOut and destroys the spawned camera.
 */
UCLASS(Blueprintable, meta=(DisplayName="Spawn and Switch Camera"))
class UNREALHELPERLIBRARY_API UANS_SpawnAndSwitchPlayerCamera : public UANS_UHL_Base
{
	GENERATED_BODY()

public:

    /** 
     * The CameraActor subclass to spawn when this notify begins.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
    TSubclassOf<ACameraActor> CameraToSpawnClass = ACameraActor::StaticClass();

	/**
	 * If set, the socket or bone name on the owning SkeletalMeshComponent to attach the spawned camera to.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
	FName AttachSocketName = NAME_None;

	/**
	 * How to attach Camera to Actor
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
	FUHLAttachmentRules AttachRules;

    /**
     * How long (in seconds) to blend into the spawned camera.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera", meta=(ClampMin="0.0"))
    float BlendInTime = 0.5f;

	/**
	 * Curve
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
	TEnumAsByte<EViewTargetBlendFunction> BlendInFunction = EViewTargetBlendFunction::VTBlend_Cubic;
	
    /**
     * How long (in seconds) to blend back to the player pawn when the notify ends.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera", meta=(ClampMin="0.0"))
    float BlendOutTime = 0.5f;

	/**
	 * Curve
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
	TEnumAsByte<EViewTargetBlendFunction> BlendOutFunction = EViewTargetBlendFunction::VTBlend_Cubic;

	/** 
	 * PlayerController that should be used for camera switching
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
	int32 PlayerControllerIndex = 0;

	/** 
	 * How much time camera will live after end
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify|Camera")
	float LifeSpanAfterEnd = 3.0f;

    // UAnimNotifyState interface
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
    // End of UAnimNotifyState interface

private:
    /** The camera actor instance we spawned at NotifyBegin. */
	UPROPERTY()
    ACameraActor* SpawnedCameraInstance = nullptr;

    /** The original view target we stored so we can restore at NotifyEnd. */
	UPROPERTY()
    AActor* PreviousViewTarget = nullptr;
};
