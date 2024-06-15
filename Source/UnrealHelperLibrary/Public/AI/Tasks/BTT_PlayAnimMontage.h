// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PlayAnimMontage.generated.h"

class UPlayMontageCallbackProxy;
/**
 *
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

	virtual FString GetStaticDescription() const override;

private:
	bool bIsAborting = false;
    TWeakObjectPtr<ACharacter> Character;
    /** Cached AIController owner of BehaviorTreeComponent. */
    UPROPERTY(Transient)
    TObjectPtr<AAIController> AIOwner;
    TWeakObjectPtr<UBehaviorTreeComponent> OwnerComponent;
    UPROPERTY()
    UPlayMontageCallbackProxy* PlayMontageCallbackProxy;

	UFUNCTION()
	void OnPlayMontageEnded(FName NotifyName);
    void ClearCallbacks();
};
