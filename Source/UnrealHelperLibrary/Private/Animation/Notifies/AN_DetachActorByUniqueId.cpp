// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/AN_DetachActorByUniqueId.h"

#if WITH_EDITOR
void UAN_DetachActorByUniqueId::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && 
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAN_DetachActorByUniqueId, UniqueId))
	{
		if (UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(GetOuter()))
		{
			AnimSeq->Modify();
		}
	}
}
#endif

FString UAN_DetachActorByUniqueId::GetNotifyName_Implementation() const
{
	return FString("Attach With UniqueId->") + UniqueId.ToString();
}


void UAN_DetachActorByUniqueId::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// if (!MeshComp) return;
	//
	// AActor* TargetActor = MeshComp->GetOwner();
	// if (!TargetActor) return;
	//
	// // 1) Load the class synchronously
	// UClass* ActorClass = ActorToAttach.LoadSynchronous();
	// if (!ActorClass)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Failed to load ActorToSpawn!"));
	// 	return;
	// }
	//
	// FActorSpawnParameters ActorSpawnParameters;
	// ActorSpawnParameters.Owner = TargetActor;
	// ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//
	// AActor* SpawnedActor = MeshComp->GetWorld()->SpawnActor<AActor>(ActorClass,
	// 	MeshComp->GetSocketTransform(SocketName), ActorSpawnParameters);
	//
	// if (!SpawnedActor) return;
	//
	// SpawnedActor->AttachToActor(TargetActor, AttachmentTransformRules, SocketName);
	// SpawnedActor->Tags.Append(UniqueId);
}