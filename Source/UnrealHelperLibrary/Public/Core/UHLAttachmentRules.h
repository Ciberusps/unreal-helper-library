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

USTRUCT(BlueprintType)
struct FUHLDetachmentRules
{
	GENERATED_BODY()

	/** 
	 * Which transform to preserve when detaching: 
	 * KeepRelative or KeepWorld 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detachment")
	EDetachmentRule DetachmentRule = EDetachmentRule::KeepWorld;

	/** 
	 * If true, calls Modify() on the component so that the detach 
	 * is recorded in the transaction buffer (undo/redo) 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detachment")
	bool bCallModify = false;

	/** 
	 * Convert to Unreal’s native struct, for use in DetachFromComponent / DetachFromActor 
	 */
	FDetachmentTransformRules ToEngineRules() const
	{
		return FDetachmentTransformRules(DetachmentRule, bCallModify);
	}
};