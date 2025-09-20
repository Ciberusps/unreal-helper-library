// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ANS_UHL_Base.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/EngineTypes.h"
#include "ANS_EnableRootMotionZAxisMovement.generated.h"

class ACharacter;

/**
 * Usefull for root motion jumps (characters/enemies)
 *
 * Enables MOVE_Flying mode during ANS,
 * on end if movement mode is still MOVE_Flying - changes it on MOVE_Falling
 *
 * if "bStopMontageIfLandCheckFails" - stops montage if land check failed on NotifyEnd
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_EnableRootMotionZAxisMovement : public UANS_UHL_Base
{
	GENERATED_BODY()

public:
	UANS_EnableRootMotionZAxisMovement(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnableRootMotionZAxisMovement")
	bool bStopMontageIfLandCheckFails = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnableRootMotionZAxisMovement", meta=(EditCondition = "bStopMontageIfLandCheckFails", EditConditionHides))
	float LandCheckDistance = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnableRootMotionZAxisMovement", meta=(EditCondition = "bStopMontageIfLandCheckFails", EditConditionHides))
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnableRootMotionZAxisMovement", meta=(EditCondition = "bStopMontageIfLandCheckFails", EditConditionHides))
	FMontageBlendSettings LandCheckBlendOutSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="EnableRootMotionZAxisMovement")
	bool bDebug = false;
	
#if WITH_EDITOR
    /** Override this to prevent firing this notify state type in animation editors */
    virtual bool ShouldFireInEditor() { return false; }
#endif

    virtual FLinearColor GetEditorColor() override { return FLinearColor(0.0f, 0.74f, 1.0f, 1.0f); };
    virtual FString GetNotifyName_Implementation() const override { return FString("EnableRootMotionZAxisMovement"); };

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual bool ShouldUseExperimentalUHLFeatures() const override { return true; };
	virtual void NotifyEndOrBlendOut(USkeletalMeshComponent* MeshComp) override;
	
private:
    EMovementMode InitialMovementMode = EMovementMode::MOVE_None;
};
