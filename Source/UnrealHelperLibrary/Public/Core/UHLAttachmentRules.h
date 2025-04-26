#pragma once

#include "UHLAttachmentRules.generated.h"

USTRUCT(BlueprintType)
struct FUHLAttachmentRules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Attachment")
	EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;

	UPROPERTY(EditAnywhere, Category="Attachment")
	EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;

	UPROPERTY(EditAnywhere, Category="Attachment")
	EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;

	UPROPERTY(EditAnywhere, Category="Attachment")
	bool bWeldSimulatedBodies = true;

	// Conversion to engine struct
	FAttachmentTransformRules ToEngineRules() const
	{
		return FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, bWeldSimulatedBodies);
	}
};