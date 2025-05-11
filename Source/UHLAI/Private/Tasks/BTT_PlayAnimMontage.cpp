// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/BTT_PlayAnimMontage.h"

#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_PlayAnimMontage)

DEFINE_LOG_CATEGORY(LogBTT_PlayAnimMontage);

UBTT_PlayAnimMontage::UBTT_PlayAnimMontage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "PlayAnimMontage";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIOwner = OwnerComp.GetAIOwner();
	ACharacter* Character = AIOwner.IsValid() ? AIOwner->GetCharacter() : nullptr;

	bool bPlayMontage = PlayMontage(
		Character ? Character->GetMesh() : nullptr,
		AnimMontage,
		PlayRate,
		StartingPosition,
		StartSectionName
	);

	return bPlayMontage ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_PlayAnimMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ClearMontageDelegate();

	if (StopMontageOnAbort && AIOwner.IsValid())
	{
		if (ACharacter* Character = AIOwner->GetCharacter())
		{
			Character->StopAnimMontage();
		}
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_PlayAnimMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,EBTNodeResult::Type TaskResult)
{
	ClearMontageDelegate();
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

FString UBTT_PlayAnimMontage::GetStaticDescription() const
{
	return FString::Printf(
		TEXT(
		"AnimMontage: %s\n"
		"PlayRate: %f\n"
		"StartingPosition: %f\n"
		"StartSectionName: %s\n"
		"StopMontageOnAbort: %s\n"
		"TaskNodeEndPolicy: %s"),
		AnimMontage ? *AnimMontage->GetName() : TEXT("None"),
		PlayRate,
		StartingPosition,
		*StartSectionName.ToString(),
		StopMontageOnAbort ? TEXT("True") : TEXT("False"),
		TaskNodeEndPolicy == ETaskNodeEndPolicy::OnMontageEnded ? TEXT("OnMontageEnded") : TEXT("OnMontageBlendingOut")
	);
}


bool UBTT_PlayAnimMontage::PlayMontage(USkeletalMeshComponent* InSkeletalMeshComponent, UAnimMontage* MontageToPlay,
                                       float InPlayRate, float InStartingPosition, FName InStartingSection)
{
	bool bPlayedSuccessfully = false;

	if (UAnimInstance* AnimInstance = InSkeletalMeshComponent ? InSkeletalMeshComponent->GetAnimInstance() : nullptr)
	{
		const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, InPlayRate,EMontagePlayReturnType::MontageLength,InStartingPosition);

		bPlayedSuccessfully = (MontageLength > 0.f);

		if (bPlayedSuccessfully)
		{
			if (InStartingSection != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(InStartingSection, MontageToPlay);
			}

			/**
			 * If a callback is set within this Tick, the notification of the previous montage interrupted will be called!
			 * We need to consume a Tick to prevent the notification of the previous montage interrupted
			 * from triggering on the current launched montage
			 */
			GetWorld()->GetTimerManager().SetTimerForNextTick([&]()
			{
				ACharacter* Character = AIOwner.IsValid() ? AIOwner->GetCharacter() : nullptr;
				USkeletalMeshComponent* MeshComponent = Character ? Character->GetMesh() : nullptr;
				UAnimInstance* AnimIns = MeshComponent ? MeshComponent->GetAnimInstance() : nullptr;
				if (AnimIns)
				{
					if (TaskNodeEndPolicy == ETaskNodeEndPolicy::OnMontageBlendingOut)
					{
						AnimIns->OnMontageBlendingOut.AddUniqueDynamic(
							this, &UBTT_PlayAnimMontage::OnMontageBlendingOut);
					}
					else if (TaskNodeEndPolicy == ETaskNodeEndPolicy::OnMontageEnded)
					{
						AnimIns->OnMontageEnded.AddUniqueDynamic(this, &UBTT_PlayAnimMontage::OnMontageEnded);
					}
				}
			});
		}
	}

	return bPlayedSuccessfully;
}


void UBTT_PlayAnimMontage::OnMontageEnded(UAnimMontage* InAnimMontage, bool bInterrupted)
{
	if (InAnimMontage != AnimMontage)
	{
		return;
	}

	FinishTaskNode();
}

void UBTT_PlayAnimMontage::OnMontageBlendingOut(UAnimMontage* InAnimMontage, bool bInterrupted)
{
	if (InAnimMontage != AnimMontage)
	{
		return;
	}

	FinishTaskNode();
}

void UBTT_PlayAnimMontage::FinishTaskNode()
{
	UBehaviorTreeComponent* OwnerComp = AIOwner.IsValid()? Cast<UBehaviorTreeComponent>(AIOwner->BrainComponent): nullptr;

	ClearMontageDelegate();

	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_PlayAnimMontage::ClearMontageDelegate()
{
	if (AIOwner.IsValid())
	{
		ACharacter* Character = AIOwner->GetCharacter();
		USkeletalMeshComponent* MeshComponent = Character ? Character->GetMesh() : nullptr;
		UAnimInstance* AnimInstance = MeshComponent ? MeshComponent->GetAnimInstance() : nullptr;

		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveAll(this);
			AnimInstance->OnMontageBlendingOut.RemoveAll(this);
		}
	}
}
