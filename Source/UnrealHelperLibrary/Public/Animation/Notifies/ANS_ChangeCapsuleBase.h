// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ANS_UHL_Base.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "CoreMinimal.h"
#include "AlphaBlend.h"
#include "Curves/CurveFloat.h"
#include "ANS_ChangeCapsuleBase.generated.h"

USTRUCT(BlueprintType)
struct FChangeCapsuleCollisionSettings
{
    GENERATED_BODY()

    /** Override Collision Enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(InlineEditConditionToggle))
    bool bOverrideCollisionEnabled = false;
    /** Collision enabled state (None / QueryOnly / QueryAndPhysics) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(EditCondition="bOverrideCollisionEnabled"))
    TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled = ECollisionEnabled::QueryAndPhysics;

    /** Override Collision Profile Name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(InlineEditConditionToggle))
    bool bOverrideCollisionProfileName = false;
    /** Collision profile name from your Project Settings → Collision */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(EditCondition="bOverrideCollisionProfileName"))
    FName CollisionProfileName = TEXT("Pawn");

    /** Override Generate Overlap Events */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(InlineEditConditionToggle))
    bool bOverrideGenerateOverlapEvents = false;
    /** Generate overlap events? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(EditCondition="bOverrideGenerateOverlapEvents"))
    bool bGenerateOverlapEvents = true;

    /** Override Force Query Only */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(InlineEditConditionToggle))
    bool bOverrideForceQueryOnly = false;
    /** If true, forces query‑only even if CollisionEnabled is set to Physics */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(EditCondition="bOverrideForceQueryOnly"))
    bool bForceQueryOnly = false;

    /** Override Custom Responses */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(InlineEditConditionToggle))
    bool bOverrideCustomResponses = false;
    /** Per‑channel responses (Block / Overlap / Ignore) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule", meta=(EditCondition="bOverrideCustomResponses"))
    TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> CustomResponses;
};

/**
 * Base ANS that handles “modify capsule properties + blend in/out via FAlphaBlend
 * + optional custom EaseCurve + debug visualization.”
 * Subclasses must override FindCapsuleComponent(...) to choose which capsule to edit.
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "Change Capsule Base"))
class UNREALHELPERLIBRARY_API UANS_ChangeCapsuleBase : public UANS_UHL_Base
{
    GENERATED_BODY()

public:
    /** Change the capsule’s radius for this notify. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta=(InlineEditConditionToggle))
    bool bModifyRadius = false;

    /** Target radius to interpolate to (only if bModifyRadius == true). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule",
              meta = (EditCondition = "bModifyRadius", ClampMin = "0.0"))
    float NewRadius = 42.0f;

    /** Change the capsule’s half-height for this notify. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta=(InlineEditConditionToggle))
    bool bModifyHalfHeight = false;

    /** Target half-height to interpolate to (only if bModifyHalfHeight == true). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule",
              meta = (EditCondition = "bModifyHalfHeight", ClampMin = "0.0"))
    float NewHalfHeight = 96.0f;

    /** Change the capsule’s component scale for this notify. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta=(InlineEditConditionToggle))
    bool bModifyScale = false;

    /** Target relative scale to interpolate to (only if bModifyScale == true). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule",
              meta = (EditCondition = "bModifyScale", EditConditionInline))
    FVector NewScale = FVector(1.0f, 1.0f, 1.0f);

    /** Change the capsule’s wireframe line thickness for this notify. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta=(InlineEditConditionToggle))
    bool bModifyLineThickness = false;

    /** Target wireframe line thickness to interpolate to (only if bModifyLineThickness == true). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule",
              meta = (EditCondition = "bModifyLineThickness", ClampMin = "0.1"))
    float NewLineThickness = 1.0f;

    /** Change the capsule’s wireframe shape color for this notify. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta=(InlineEditConditionToggle))
    bool bModifyShapeColor = false;

    /** Target wireframe shape color to interpolate to (only if bModifyShapeColor == true). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule",
              meta = (EditCondition = "bModifyShapeColor", EditConditionInline))
    FLinearColor NewShapeColor = FLinearColor::Red;

    /** How long (seconds) to blend from original → target when notify begins. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta = (ClampMin = "0.0"))
    float BlendTimeIn = 0.0f;

    /** Blend function to use when blending in (Linear, Cubic, Exponential, etc.). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule")
    EAlphaBlendOption BlendOptionIn = EAlphaBlendOption::Linear;

    /** How long (seconds) to blend from target → original when notify ends. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta = (ClampMin = "0.0"))
    float BlendTimeOut = 0.0f;

    /** Blend function to use when blending out. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule")
    EAlphaBlendOption BlendOptionOut = EAlphaBlendOption::Linear;

	/** Collision settings with independent toggles */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Change Capsule")
	FChangeCapsuleCollisionSettings CapsuleCollisionSettings;
	
    /**
     * Optional custom Ease Curve:  
     * If you assign a valid FRuntimeFloatCurve here, the raw alpha (from FAlphaBlend)  
     * will be remapped through this curve. If left empty, interpolation uses FAlphaBlend only.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule", meta = (DisplayName = "Custom Ease Curve (Optional)"))
    FRuntimeFloatCurve EaseCurve;

    /** If true, draw a debug visualization of the capsule each tick. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Capsule")
    bool bDebug = false;


    virtual void NotifyBegin(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float TotalDuration,
        const FAnimNotifyEventReference& EventReference
    ) override;

    virtual void NotifyTick(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float FrameDeltaTime,
        const FAnimNotifyEventReference& EventReference
    ) override;

    virtual void NotifyEnd(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        const FAnimNotifyEventReference& EventReference
    ) override;


protected:
    /** 
     * Subclasses must return the UCapsuleComponent to modify (e.g. Character’s capsule or named capsule). 
     * Return nullptr if none found.
     */
    virtual UCapsuleComponent* FindCapsuleComponent(USkeletalMeshComponent* MeshComp) const PURE_VIRTUAL(UANS_ChangeCapsuleBase::FindCapsuleComponent, return nullptr;);


private:
    /** The capsule component we’ll modify. */
    UPROPERTY(Transient)
    UCapsuleComponent* CapsuleComp = nullptr;

    /** Stored “original” values from NotifyBegin. */
    float OriginalRadius = 0.0f;
    float OriginalHalfHeight = 0.0f;
    FVector OriginalScale = FVector::OneVector;
    float OriginalLineThickness = 1.0f;
    FLinearColor OriginalShapeColor = FLinearColor::White;

    /** Cached “target” values (NewX if flagged; otherwise OriginalX). */
    float TargetRadius = 0.0f;
    float TargetHalfHeight = 0.0f;
    FVector TargetScale = FVector::OneVector;
    float TargetLineThickness = 1.0f;
    FLinearColor TargetShapeColor = FLinearColor::White;

    /** FAlphaBlend instances for in/out. */
    FAlphaBlend BlendInAlpha;
    FAlphaBlend BlendOutAlpha;

    /** Total duration of this notify (from NotifyBegin). */
    float NotifyTotalDuration = 0.0f;

    /** Elapsed time (since NotifyBegin). */
    float ElapsedTime = 0.0f;

    /** Whether we successfully grabbed “originals” in NotifyBegin. */
    bool bHasValidOriginals = false;

	/** Snapshot of original capsule settings for revert */
	FChangeCapsuleCollisionSettings OriginalCapsuleSettings;

	// Save original collision settings
	void SaveOriginalCollisionSettings();
	// Restore capsule to original state
	void RestoreOriginalCollisionSettings();
	// Applies settings: restores original then applies overrides
	void ApplyCollisionSettings();
};