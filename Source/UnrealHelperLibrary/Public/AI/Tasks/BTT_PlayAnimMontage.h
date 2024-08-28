// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayMontageCallbackProxy.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PlayAnimMontage.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBTT_PlayAnimMontage, Log, All);

/**
 * Deciding when to end the node
 */
UENUM()
enum ETaskNodeEndPolicy
{
	/**
	 * after the node ends, still need to perform montage node, using this policy can make the anime more coherent
	 */
	OnMontageBlendingOut,

	/**
	 * end when the montage is completely finished, applicable to situations where only one montage is executed
	 */
	OnMontageEnded
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
	UPROPERTY(Category="Blackboard", EditAnywhere)
	bool StopMontageOnAbort = true;
	UPROPERTY(Category="Blackboard", EditAnywhere)
	TEnumAsByte<ETaskNodeEndPolicy> TaskNodeEndPolicy = ETaskNodeEndPolicy::OnMontageBlendingOut;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	                            EBTNodeResult::Type TaskResult) override;

	virtual FString GetStaticDescription() const override;

private:
	/**
	 * AIOwner Cache
	 */
	UPROPERTY()
	TWeakObjectPtr<AAIController> AIOwner;

protected:
	/**
	 * PlayMontage and bind callback
	 */
	bool PlayMontage(USkeletalMeshComponent* InSkeletalMeshComponent,
	                 UAnimMontage* MontageToPlay,
	                 float InPlayRate,
	                 float InStartingPosition,
	                 FName InStartingSection);

	/**
	 * All callbacks at the end of montage need to be filtered
	 */
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* InAnimMontage, bool bInterrupted);

	/**
	 * All callbacks at the BlendingOut of montage need to be filtered
	 */
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* InAnimMontage, bool bInterrupted);

	void ClearMontageDelegate();

	/**
	 * finish AIOwner task node, if AIOwner still is Valid
	 */
	void FinishTaskNode();
};
