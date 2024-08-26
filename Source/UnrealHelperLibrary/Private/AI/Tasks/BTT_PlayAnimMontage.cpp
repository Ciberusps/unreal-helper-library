// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_PlayAnimMontage.h"

#include "AIController.h"
#include "PlayMontageCallbackProxy.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogBTT_PlayAnimMontage);

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
	
    AIOwner = OwnerComp.GetAIOwner();
	ACharacter* Character = AIOwner.IsValid() ? AIOwner->GetCharacter() : nullptr;
	
	if (!Character)
	{
	    return Result;
	}
	
    PlayMontageCallbackProxy = UMyPlayMontageCallbackProxy::TryCreateProxyObjectForPlayMontage(
		Character->GetMesh(),
		AnimMontage,
		PlayRate,
		StartingPosition,
		StartSectionName
	);

	//Play Montage Success
	if(PlayMontageCallbackProxy)
	{
		Result = EBTNodeResult::InProgress;
		PlayMontageCallbackProxy->OnCompleted.AddUniqueDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);
		PlayMontageCallbackProxy->OnInterrupted.AddUniqueDynamic(this, &UBTT_PlayAnimMontage::OnPlayMontageEnded);
	}
	else
	{
		UE_LOG(LogBTT_PlayAnimMontage, Warning, TEXT("Montage(%s) Play Failed!"), *GetNameSafe(AnimMontage));
		Result = EBTNodeResult::Failed;
	}
	
    return Result;
}

EBTNodeResult::Type UBTT_PlayAnimMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (StopMontageOnAbort && AIOwner.IsValid())
    {
	    if(ACharacter* Character = AIOwner->GetCharacter())
    	{
    		Character->StopAnimMontage();
	    	
	    	//Clear MontageCallback
	    	if(PlayMontageCallbackProxy)
	    	{
	    		PlayMontageCallbackProxy->BeginDestroy();
	    	}
    	}
    }
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_PlayAnimMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if(PlayMontageCallbackProxy)
	{
		PlayMontageCallbackProxy->BeginDestroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s(%d): OnTaskFinished， OwnerComp = %p"), TEXT(__FUNCTION__), __LINE__, &OwnerComp );
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

FString UBTT_PlayAnimMontage::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), AnimMontage ? *AnimMontage->GetName() : TEXT(""));
}

void UBTT_PlayAnimMontage::OnPlayMontageEnded(FName NotifyName)
{
	UBehaviorTreeComponent* OwnerComp = AIOwner.IsValid() ? Cast<UBehaviorTreeComponent>(AIOwner->BrainComponent) : nullptr;

	if(OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}

	if(PlayMontageCallbackProxy)
	{
		PlayMontageCallbackProxy->OnCompleted.RemoveAll(this);
		PlayMontageCallbackProxy->OnInterrupted.RemoveAll(this);
	}
}
