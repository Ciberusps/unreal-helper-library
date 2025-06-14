// Pavel Penkov 2025 All Rights Reserved.

#include "Animation/Notifies/ANS_ChangeCapsuleBase.h"

#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

void UANS_ChangeCapsuleBase::NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    float TotalDuration,
    const FAnimNotifyEventReference& EventReference
)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    ElapsedTime = 0.0f;
    NotifyTotalDuration = TotalDuration;
    bHasValidOriginals = false;
    CapsuleComp = nullptr;

    if (!MeshComp)
    {
        return;
    }

    // Let the subclass find its capsule component
    CapsuleComp = FindCapsuleComponent(MeshComp);
    if (!CapsuleComp)
    {
        return;
    }

    // Store “original” values immediately
    OriginalRadius        = CapsuleComp->GetUnscaledCapsuleRadius();
    OriginalHalfHeight    = CapsuleComp->GetUnscaledCapsuleHalfHeight();
    OriginalScale         = CapsuleComp->GetRelativeScale3D();
    OriginalShapeColor    = CapsuleComp->ShapeColor.ReinterpretAsLinear();

    // Compute and cache “target” values now
    TargetRadius        = bModifyRadius       ? NewRadius        : OriginalRadius;
    TargetHalfHeight    = bModifyHalfHeight   ? NewHalfHeight    : OriginalHalfHeight;
    TargetScale         = bModifyScale        ? NewScale         : OriginalScale;
    TargetLineThickness = bModifyLineThickness ? NewLineThickness : OriginalLineThickness;
    TargetShapeColor    = bModifyShapeColor   ? NewShapeColor    : OriginalShapeColor;

    bHasValidOriginals = true;

    // --- Construct FAlphaBlend for blend-in using FAlphaBlendArgs ---
    {
        FAlphaBlendArgs InArgs;
        InArgs.BlendTime   = (BlendTimeIn > KINDA_SMALL_NUMBER) ? BlendTimeIn : 0.0f;
        InArgs.BlendOption = BlendOptionIn;
        BlendInAlpha = FAlphaBlend(InArgs);
    }

    // --- Construct FAlphaBlend for blend-out using FAlphaBlendArgs ---
    {
        FAlphaBlendArgs OutArgs;
        OutArgs.BlendTime   = (BlendTimeOut > KINDA_SMALL_NUMBER) ? BlendTimeOut : 0.0f;
        OutArgs.BlendOption = BlendOptionOut;
        BlendOutAlpha = FAlphaBlend(OutArgs);
    }

    // If instant blend-in, snap right away to the “target” values
    if (BlendTimeIn <= KINDA_SMALL_NUMBER)
    {
        if (bModifyRadius)
        {
            CapsuleComp->SetCapsuleRadius(TargetRadius, /*bUpdateOverlaps=*/ false);
        }
        if (bModifyHalfHeight)
        {
            CapsuleComp->SetCapsuleHalfHeight(TargetHalfHeight, /*bUpdateOverlaps=*/ false);
        }
        if (bModifyScale)
        {
            CapsuleComp->SetRelativeScale3D(TargetScale);
        }
        if (bModifyLineThickness)
        {
            CapsuleComp->SetLineThickness(TargetLineThickness);
        }
        if (bModifyShapeColor)
        {
            CapsuleComp->ShapeColor = TargetShapeColor.ToFColor(/*bSRGB=*/ true);
        }
    }
    // Otherwise, leave them at “original” until NotifyTick interpolates

	SaveOriginalCollisionSettings();
	ApplyCollisionSettings();
}

void UANS_ChangeCapsuleBase::NotifyTick(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    float FrameDeltaTime,
    const FAnimNotifyEventReference& EventReference
)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    if (!bHasValidOriginals || !CapsuleComp)
    {
        return;
    }

    ElapsedTime += FrameDeltaTime;

    // 1) Compute “raw alpha” in [0..1] based on blend-in / hold / blend-out
    float RawAlpha = 1.0f;

    // a) Blend-in phase
    if (BlendTimeIn > KINDA_SMALL_NUMBER && ElapsedTime < BlendTimeIn)
    {
        BlendInAlpha.Update(FrameDeltaTime);
        RawAlpha = BlendInAlpha.GetBlendedValue();
    }
    // b) Hold at target (after blend-in, before blend-out)
    else if (ElapsedTime >= BlendTimeIn && ElapsedTime <= (NotifyTotalDuration - BlendTimeOut))
    {
        RawAlpha = 1.0f;
    }
    // c) Blend-out phase
    else if (BlendTimeOut > KINDA_SMALL_NUMBER && ElapsedTime > (NotifyTotalDuration - BlendTimeOut))
    {
        float OutElapsed = ElapsedTime - (NotifyTotalDuration - BlendTimeOut);
        BlendOutAlpha.Update(FrameDeltaTime);
        RawAlpha = 1.0f - BlendOutAlpha.GetBlendedValue();
    }
    // d) Otherwise (zero‐blend cases), RawAlpha stays 1.0

    RawAlpha = FMath::Clamp(RawAlpha, 0.0f, 1.0f);

    // 2) If user provided an EaseCurve, remap RawAlpha
    if (EaseCurve.GetRichCurveConst() && EaseCurve.GetRichCurveConst()->GetNumKeys() > 0)
    {
        float Curved = EaseCurve.GetRichCurveConst()->Eval(RawAlpha);
        RawAlpha = FMath::Clamp(Curved, 0.0f, 1.0f);
    }

    // 3) Lerp each property from “original” → “target” using RawAlpha
    float CurrentRadius        = OriginalRadius;
    float CurrentHalfHeight    = OriginalHalfHeight;
    FVector CurrentScale       = OriginalScale;
    float CurrentLineThickness = OriginalLineThickness;
    FLinearColor CurrentShapeColor = OriginalShapeColor;

    if (bModifyRadius)
    {
        CurrentRadius = FMath::Lerp(OriginalRadius, TargetRadius, RawAlpha);
        CapsuleComp->SetCapsuleRadius(CurrentRadius, /*bUpdateOverlaps=*/ false);
    }
    if (bModifyHalfHeight)
    {
        CurrentHalfHeight = FMath::Lerp(OriginalHalfHeight, TargetHalfHeight, RawAlpha);
        CapsuleComp->SetCapsuleHalfHeight(CurrentHalfHeight, /*bUpdateOverlaps=*/ false);
    }
    if (bModifyScale)
    {
        CurrentScale = FMath::Lerp(OriginalScale, TargetScale, RawAlpha);
        CapsuleComp->SetRelativeScale3D(CurrentScale);
    }
    if (bModifyLineThickness)
    {
        CurrentLineThickness = FMath::Lerp(OriginalLineThickness, TargetLineThickness, RawAlpha);
        CapsuleComp->SetLineThickness(CurrentLineThickness);
    }
    if (bModifyShapeColor)
    {
        CurrentShapeColor = FMath::Lerp(OriginalShapeColor, TargetShapeColor, RawAlpha);
        CapsuleComp->ShapeColor = CurrentShapeColor.ToFColor(/*bSRGB=*/ true);
    }

    // 4) If bDebug is true, draw a wireframe debug capsule using the current interpolated values
    if (bDebug && CapsuleComp)
    {
        FVector Location = CapsuleComp->GetComponentLocation();
        FRotator Rotation = CapsuleComp->GetComponentRotation();
        const FVector CompScale = CapsuleComp->GetComponentScale();
        float DrawRadius = CurrentRadius * FMath::Max(CompScale.X, CompScale.Y);
        float DrawHalfHeight = CurrentHalfHeight * CompScale.Z;
        FColor DebugColor = FColor::Yellow;
        float LifeTime = 0.0f;    // one frame
        uint8 DepthPriority = 0;  // Normal
        float Thickness = 1.0f;

        DrawDebugCapsule(
            CapsuleComp->GetWorld(),
            Location,
            DrawHalfHeight,
            DrawRadius,
            Rotation.Quaternion(),
            DebugColor,
            false,
            LifeTime,
            DepthPriority,
            Thickness
        );
    }
}

void UANS_ChangeCapsuleBase::NotifyEnd(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference
)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!bHasValidOriginals || !CapsuleComp)
    {
        return;
    }

    // Always restore every modified property back to its original value
    if (bModifyRadius)
    {
        CapsuleComp->SetCapsuleRadius(OriginalRadius, /*bUpdateOverlaps=*/ false);
    }
    if (bModifyHalfHeight)
    {
        CapsuleComp->SetCapsuleHalfHeight(OriginalHalfHeight, /*bUpdateOverlaps=*/ false);
    }
    if (bModifyScale)
    {
        CapsuleComp->SetRelativeScale3D(OriginalScale);
    }
    if (bModifyLineThickness)
    {
        CapsuleComp->SetLineThickness(OriginalLineThickness);
    }
    if (bModifyShapeColor)
    {
        CapsuleComp->ShapeColor = OriginalShapeColor.ToFColor(/*bSRGB=*/ true);
    }

    // Clear internal pointers & flags
    CapsuleComp = nullptr;
    bHasValidOriginals = false;
    ElapsedTime = 0.0f;
    NotifyTotalDuration = 0.0f;

	RestoreOriginalCollisionSettings();
}

void UANS_ChangeCapsuleBase::SaveOriginalCollisionSettings()
{
	if (!CapsuleComp) return;

	// Collision Enabled
	OriginalCapsuleSettings.CollisionEnabled = CapsuleComp->GetCollisionEnabled();
	OriginalCapsuleSettings.bForceQueryOnly = false;
	OriginalCapsuleSettings.bOverrideCollisionEnabled = true;

	// Profile Name
	OriginalCapsuleSettings.CollisionProfileName = CapsuleComp->GetCollisionProfileName();
	OriginalCapsuleSettings.bOverrideCollisionProfileName = true;

	// Overlap Events
	OriginalCapsuleSettings.bGenerateOverlapEvents = CapsuleComp->GetGenerateOverlapEvents();
	OriginalCapsuleSettings.bOverrideGenerateOverlapEvents = true;

	// Custom Responses
	OriginalCapsuleSettings.CustomResponses.Empty();
	for (int32 Chan = 0; Chan < ECollisionChannel::ECC_MAX; ++Chan)
	{
		OriginalCapsuleSettings.CustomResponses.Add(
			static_cast<ECollisionChannel>(Chan),
			CapsuleComp->GetCollisionResponseToChannel((ECollisionChannel)Chan)
		);
	}
	OriginalCapsuleSettings.bOverrideCustomResponses = true;
}

void UANS_ChangeCapsuleBase::RestoreOriginalCollisionSettings()
{
	if (!CapsuleComp) return;

	// Restore enabled & profile
	CapsuleComp->SetCollisionEnabled(OriginalCapsuleSettings.CollisionEnabled);
	CapsuleComp->SetCollisionProfileName(OriginalCapsuleSettings.CollisionProfileName);

	// Restore overlap
	CapsuleComp->SetGenerateOverlapEvents(OriginalCapsuleSettings.bGenerateOverlapEvents);

	// Restore responses
	for (auto& Pair : OriginalCapsuleSettings.CustomResponses)
	{
		CapsuleComp->SetCollisionResponseToChannel(Pair.Key, Pair.Value);
	}
}

void UANS_ChangeCapsuleBase::ApplyCollisionSettings()
{
	if (!CapsuleComp) return;

	// Apply overrides
	if (CapsuleCollisionSettings.bOverrideCollisionEnabled)
	{
		// Use if-else instead of ternary to prevent ambiguity
		ECollisionEnabled::Type NewMode = CapsuleCollisionSettings.CollisionEnabled;
		if (CapsuleCollisionSettings.bForceQueryOnly)
		{
			NewMode = ECollisionEnabled::QueryOnly;
		}
		CapsuleComp->SetCollisionEnabled(NewMode);
	}
	if (CapsuleCollisionSettings.bOverrideCollisionProfileName)
	{
		CapsuleComp->SetCollisionProfileName(CapsuleCollisionSettings.CollisionProfileName);
	}
	if (CapsuleCollisionSettings.bOverrideGenerateOverlapEvents)
	{
		CapsuleComp->SetGenerateOverlapEvents(CapsuleCollisionSettings.bGenerateOverlapEvents);
	}
	if (CapsuleCollisionSettings.bOverrideCustomResponses)
	{
		for (auto& Pair : CapsuleCollisionSettings.CustomResponses)
		{
			CapsuleComp->SetCollisionResponseToChannel(Pair.Key, Pair.Value);
		}
	}
}
