// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/AN_UHL_Base.h"
#include "AN_DetachActorByUniqueId.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAN_DetachActorByUniqueId : public UAN_UHL_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="DetachActorById")
	AActor* ActorToAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttachWithUniqueId")
	FName UniqueId = "Unique_ID";

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FF7DE7"); };
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	
};
