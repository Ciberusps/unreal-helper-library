// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/AN_AttachWithUniqueId.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AN_AttachWithUniqueId)

#if WITH_EDITOR
void UAN_AttachWithUniqueId::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && 
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAN_AttachWithUniqueId, UniqueId))
	{
		if (UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(GetOuter()))
		{
			AnimSeq->Modify();
		}
	}
}
#endif

FString UAN_AttachWithUniqueId::GetNotifyName_Implementation() const
{
	return FString("Attach With UniqueId -> ") + UniqueId.ToString();
}

void UAN_AttachWithUniqueId::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	
	AActor* TargetActor = MeshComp->GetOwner();
	if (!TargetActor) return;

	// 1) Load the class synchronously
	UClass* ActorClass = ActorToAttach.LoadSynchronous();
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load ActorToSpawn!"));
		return;
	}

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = TargetActor;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = MeshComp->GetWorld()->SpawnActor<AActor>(ActorClass,
		MeshComp->GetSocketTransform(SocketName), ActorSpawnParameters);

	if (!SpawnedActor) return;

	SpawnedActor->AttachToComponent(MeshComp, AttachmentRules.ToEngineRules(), SocketName);
	SpawnedActor->Tags.Add(UniqueId);
}