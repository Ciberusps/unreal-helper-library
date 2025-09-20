// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animations/Notifies/ANS_ActivateAbility.h"
#include "ANS_UHLGAS_GlobalTimeDilation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(ToolTip="Controls when GlobalTimeDilation can be changed by GlobalTimeDilation Curve"))
class UHLGAS_API UANS_UHLGAS_GlobalTimeDilation : public UANS_UHL_Base
{
	GENERATED_BODY()
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) override;

	virtual FLinearColor GetEditorColor() override { return FLinearColor(232/256.0f, 229/256.0f, 139/256.0f); };
	virtual FString GetNotifyName_Implementation() const override { return FString("GlobalTimeDilation"); };
};
