// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notifies/AN_UHL_Base.h"
#include "Core/UHLAttachmentRules.h"
#include "Engine/EngineTypes.h"
#include "AN_AttachActorWithUniqueId.generated.h"

/**
 * Spawns an actor of the specified class, attaches it to a given component socket,
 * and tags it with a user-defined unique identifier.  The tag allows you to
 * later retrieve this exact attached actor by searching your owner’s attached
 * actors for the matching tag.
 *
 * Typical usage:
 *   1. During an animation or setup phase, call AN_AttachActorWithUniqueId to
 *      spawn and attach, for example, a projectile to your character mesh:
 *        - It spawns the projectile actor at the socket transform.
 *        - It adds a tag (e.g. “Projectile_42”) so you can distinguish it.
 *   2. Later (e.g. when firing), call LaunchProjectile:
 *        - It searches the character’s attached actors for the tag “Projectile_42”.
 *        - It finds the correct spawned actor and applies your launch logic.
 *
 * @param ActorToAttach        The UClass of the actor to spawn and attach.
 * @param SocketName        The name of the socket on OwnerComponent to attach to.
 * @param UniqueId          A user-defined string used as a tag on the spawned actor.
 * @param AttachmentTransformRules    Attachment rules
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAN_AttachActorWithUniqueId : public UAN_UHL_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="AttachActorWithUniqueId")
	TSoftClassPtr<AActor> ActorToAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttachActorWithUniqueId")
	FName UniqueId = "Unique_ID";
	
	UPROPERTY(EditAnywhere, Category="AttachActorWithUniqueId", meta = (AnimNotifyBoneName = "true"))
	FName SocketName = "";

	UPROPERTY(EditAnywhere, Category="AttachActorWithUniqueId")
	FUHLAttachmentRules AttachmentRules;

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
