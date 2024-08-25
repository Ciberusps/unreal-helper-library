// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_PlayAnimMontage.h"

#include "AIController.h"
#include "PlayMontageCallbackProxy.h"
#include "GameFramework/Character.h"

UBTT_PlayAnimMontage::UBTT_PlayAnimMontage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "PlayAnimMontage";
    // TODO try to find the way to remove instancing
    // instantiating to be able to use Timers
    bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;

	bIsAborting = false;
    FUHLPlayAnimMontageMemory* MyMemory = CastInstanceNodeMemory<FUHLPlayAnimMontageMemory>(NodeMemory);
    AAIController* AIOwner = OwnerComp.GetAIOwner();
    OwnerComponent = &OwnerComp;
    if (!AIOwner)
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

    MyMemory->PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
        Character->GetMesh(),
        AnimMontage,
        PlayRate,
        StartingPosition,
        StartSectionName
    );
    MyMemory->PlayMontageCallbackProxy->OnCompleted.AddUniqueDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);
    MyMemory->PlayMontageCallbackProxy->OnInterrupted.AddUniqueDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);
    MyMemory->PlayMontageCallbackProxy->OnBlendOut.AddUniqueDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);

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

void UBTT_PlayAnimMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    FUHLPlayAnimMontageMemory* MyMemory = CastInstanceNodeMemory<FUHLPlayAnimMontageMemory>(NodeMemory);
    MyMemory->PlayMontageCallbackProxy->OnCompleted.RemoveAll(this);
    MyMemory->PlayMontageCallbackProxy->OnInterrupted.RemoveAll(this);
    MyMemory->PlayMontageCallbackProxy->OnBlendOut.RemoveAll(this);

    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

FString UBTT_PlayAnimMontage::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), AnimMontage ? *AnimMontage->GetName() : TEXT(""));
}

void UBTT_PlayAnimMontage::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
    Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
}

void UBTT_PlayAnimMontage::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
    Super::CleanupMemory(OwnerComp, NodeMemory, CleanupType);
}

void UBTT_PlayAnimMontage::OnPlayMontageEnded(FName NotifyName)
{
	const EBTNodeResult::Type NodeResult(EBTNodeResult::Succeeded);

	if (OwnerComponent && !bIsAborting)
	{
		FinishLatentTask(*OwnerComponent, NodeResult);
	}
}
