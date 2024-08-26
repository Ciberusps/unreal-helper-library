// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayMontageCallbackProxy.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PlayAnimMontage.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBTT_PlayAnimMontage,Log,All);

UCLASS()
class UMyPlayMontageCallbackProxy : public UPlayMontageCallbackProxy
{
	GENERATED_BODY()

public:
	
	/**
	 * try to Play montage and create callback.
	 * If played successfully, return callback. otherwise return null
	 */
	static UMyPlayMontageCallbackProxy* TryCreateProxyObjectForPlayMontage(
		USkeletalMeshComponent* InSkeletalMeshComponent, 
		UAnimMontage* MontageToPlay, 
		float PlayRate = 1.f, 
		float StartingPosition = 0.f, 
		FName StartingSection = NAME_None)
	{
		UMyPlayMontageCallbackProxy* Proxy = NewObject<UMyPlayMontageCallbackProxy>();
		
		if(Proxy->PlayMontage(InSkeletalMeshComponent, MontageToPlay, PlayRate, StartingPosition, StartingSection))
		{
			Proxy->SetFlags(RF_StrongRefOnFrame);
			return Proxy;
		}
		
		Proxy->MarkAsGarbage();
		return nullptr;
	}
};

/**
 * playMontage and wait finish.
 * If the montage is already playing, it will be forcefully overwritten and played directly.
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTT_PlayAnimMontage : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_PlayAnimMontage(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category="Blackboard", EditAnywhere)
	UAnimMontage* AnimMontage = nullptr;
	UPROPERTY(Category="Blackboard", EditAnywhere)
	float PlayRate = 1.0f;
	UPROPERTY(Category="Blackboard", EditAnywhere)
	float StartingPosition = 0.0f;
	UPROPERTY(Category="Blackboard", EditAnywhere)
	FName StartSectionName = NAME_None;
	// TODO probably should be true by default
	UPROPERTY(Category="Blackboard", EditAnywhere)
	bool StopMontageOnAbort = false;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual FString GetStaticDescription() const override;

private:
	
	/**
	 * AIOwner Caceh
	 */
	UPROPERTY()
	TWeakObjectPtr<AAIController> AIOwner;
	
	/**
	 * MontageCallback, on executeTask assignment
	 */
	UPROPERTY()
	TObjectPtr<UMyPlayMontageCallbackProxy> PlayMontageCallbackProxy;

private:
	
	UFUNCTION()
	void OnPlayMontageEnded(FName NotifyName);
};
