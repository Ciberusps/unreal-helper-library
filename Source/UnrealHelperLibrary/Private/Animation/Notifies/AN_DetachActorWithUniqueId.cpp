// Pavel Penkov 2025 All Rights Reserved.


#include "Animation/Notifies/AN_DetachActorWithUniqueId.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#if WITH_EDITOR
void UAN_DetachActorWithUniqueId::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && 
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAN_DetachActorWithUniqueId, UniqueId))
	{
		if (UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(GetOuter()))
		{
			AnimSeq->Modify();
		}
	}
}
#endif

FString UAN_DetachActorWithUniqueId::GetNotifyName_Implementation() const
{
	return FString("Detach With UniqueId->") + UniqueId.ToString();
}

void UAN_DetachActorWithUniqueId::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;

	AActor* AttachedActor = UUnrealHelperLibraryBPL::FindAttachedActorByTag(OwnerActor, UniqueId);
	if (!AttachedActor) return;

	AttachedActor->DetachFromActor(DetachmentRules.ToEngineRules());
	
	// checking that physics enabled before AutoDestroy and it worth to create timer 
	if (bEnablePhysicsOnDetach && EnablePhysicsDelay < AutoDestroyDelay)
	{
		FTimerDelegate Delegate;
		Delegate.BindLambda([AttachedActor]()
		{
			// TODO: improve detachment logic
			TArray<UActorComponent*> Comps = AttachedActor->K2_GetComponentsByClass(UPrimitiveComponent::StaticClass());
			for (UActorComponent* Comp : Comps)
			{
				if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Comp))
				{
					Prim->SetCollisionProfileName(TEXT("PhysicsActor"));
				    Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					Prim->SetSimulatePhysics(true);
					// Prim->SetEnableGravity(true);
					// // ensure it has a physics‐friendly collision profile
				    // Prim->SetCollisionProfileName(TEXT("PhysicsActor"));
				    // Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				    //
				    // // turn on physics and wake it up
				    // Prim->SetSimulatePhysics(true);
				    // Prim->WakeAllRigidBodies();
		
				    // 3) (Optional) give it a little kick
				    const FVector Impulse = AttachedActor->GetActorForwardVector() * 200.0f
										 + FVector::UpVector * 100.0f;
				    Prim->AddImpulse(Impulse, NAME_None, /*bVelChange=*/ true);
				}
			}
		});
		MeshComp->GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, EnablePhysicsDelay, false);
	}
	
	if (bAutoDestroy)
	{
		AttachedActor->SetLifeSpan(AutoDestroyDelay);
	}
}