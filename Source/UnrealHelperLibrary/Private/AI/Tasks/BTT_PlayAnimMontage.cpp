// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_PlayAnimMontage.h"

#include "PlayMontageCallbackProxy.h"
#include "GameFramework/Character.h"

UBTT_PlayAnimMontage::UBTT_PlayAnimMontage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "PlayAnimMontage";
}

EBTNodeResult::Type UBTT_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Character = Cast<ACharacter>(OwnerComp.GetOwner());
	if (!Character.IsValid()) return EBTNodeResult::Failed;

	UPlayMontageCallbackProxy* PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
		Character->GetMesh(),
		AnimMontage,
		PlayRate,
		StartingPosition,
		StartSectionName
	);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);
	PlayMontageCallbackProxy->OnBlendOut.AddDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_PlayAnimMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsAborting = true;
    if (StopMontageOnAbort && Character.IsValid())
    {
        Character.Get()->StopAnimMontage();
    }
	return Super::AbortTask(OwnerComp, NodeMemory);
}

FString UBTT_PlayAnimMontage::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), AnimMontage ? *AnimMontage->GetName() : TEXT(""));
}

void UBTT_PlayAnimMontage::OnPlayMontageEnded(FName NotifyName)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	const EBTNodeResult::Type NodeResult(EBTNodeResult::Succeeded);

	if (OwnerComp && !bIsAborting)
	{
		FinishLatentTask(*OwnerComp, NodeResult);
	}
}

