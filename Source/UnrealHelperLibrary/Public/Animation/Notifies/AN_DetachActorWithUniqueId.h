// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/AN_UHL_Base.h"
#include "Core/UHLAttachmentRules.h"
#include "AN_DetachActorWithUniqueId.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAN_DetachActorWithUniqueId : public UAN_UHL_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DetachActorWithUniqueId")
	FName UniqueId = "Unique_ID";

	UPROPERTY(EditAnywhere, Category="DetachActorWithUniqueId")
	FUHLDetachmentRules DetachmentRules;

	UPROPERTY(EditAnywhere, Category="DetachActorWithUniqueId")
	bool bEnablePhysicsOnDetach = true;

	UPROPERTY(EditAnywhere, Category="DetachActorWithUniqueId", meta=(EditCondition="bEnablePhysicsOnDetach", EditConditionHides))
	float EnablePhysicsDelay = 0.2f;

	// TODO: add choosing preset or PhysicsSettings
	// UPROPERTY(EditAnywhere, Category="DetachActorWithUniqueId", meta=(EditCondition="bEnablePhysicsOnDetach", EditConditionHides))
	// float EnablePhysicsDelay = 1.0f;

	// TODO add option to choose blueprint and click on what component what phys settings should be applied

	// TODO: choosing Impulse and force of detaching

	UPROPERTY(EditAnywhere, Category="DetachActorWithUniqueId")
	bool bAutoDestroy = false;

	UPROPERTY(EditAnywhere, Category="DetachActorWithUniqueId", meta=(EditCondition="bAutoDestroy", EditConditionHides))
	float AutoDestroyDelay = 1.0f;

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FF7DE7"); };
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	

private:
	UPROPERTY()
	FName CollisionProfileName = FName("PhysicsActor");
	FTimerHandle TimerHandle;
};
