// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_PlayAnimMontage.h"

#include "AIController.h"
#include "PlayMontageCallbackProxy.h"
#include "GameFramework/Character.h"

UBTT_PlayAnimMontage::UBTT_PlayAnimMontage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "PlayAnimMontage";
}

EBTNodeResult::Type UBTT_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;

    AIOwner = OwnerComp.GetAIOwner();
    OwnerComponent = &OwnerComp;
    if (!AIOwner.Get())
    {
        Result = EBTNodeResult::Failed;
        return Result;
    }

	Character = Cast<ACharacter>(AIOwner->GetCharacter());
	if (!Character.IsValid())
	{
	    Result = EBTNodeResult::Failed;
	    return Result;
	}

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

	Result = EBTNodeResult::InProgress;
    return Result;
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
	const EBTNodeResult::Type NodeResult(EBTNodeResult::Succeeded);

	if (OwnerComponent.IsValid() && !bIsAborting)
	{
		FinishLatentTask(*OwnerComponent, NodeResult);
	}
}

