// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "ANS_UHL_Base.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/EngineTypes.h"
#include "ANS_EnableRootMotionZAxisMovement.generated.h"

class ACharacter;

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_EnableRootMotionZAxisMovement : public UANS_UHL_Base
{
	GENERATED_BODY()

public:

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
    TWeakObjectPtr<ACharacter> BaseCharacter = nullptr;
    EMovementMode InitialMovementMode = EMovementMode::MOVE_None;
};
