// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/ANS_UHL_Base.h"
#include "ANS_ChangeRotationRate.generated.h"

class UCharacterMovementComponent;

/**
 * better to use UANS_UHL_AllowCharacterRotation it combines both
 * - bAllowPhysicsRotationDuringAnimRootMotion
 * - and opportunity to change RotationRate
 * thats much common use case than just changing rotation rate
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_ChangeRotationRate : public UANS_UHL_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	FRotator RotationRate = FRotator::ZeroRotator;

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
#endif

	virtual FLinearColor GetEditorColor() override { return FLinearColor(0.53f, 0.6f, 0.85f); };
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) override;

private:
	UPROPERTY()
	FRotator InitialRotationRate = FRotator::ZeroRotator;

	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UFUNCTION()
	void ReturnInitialRotationRate();

	// UFUNCTION()
	// UUHLCharacterMovementComponent* GetUHLCharacterMovementComponent(ACharacter* Character);
};
