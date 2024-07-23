// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PlayAnimMontage.generated.h"

class UPlayMontageCallbackProxy;

struct FUHLPlayAnimMontageMemory
{
    UPlayMontageCallbackProxy* PlayMontageCallbackProxy = nullptr;
};

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
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual FString GetStaticDescription() const override;

    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FUHLPlayAnimMontageMemory); }
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

private:
	bool bIsAborting = false;
    TWeakObjectPtr<ACharacter> Character;
    /** Cached AIController owner of BehaviorTreeComponent. */
    UPROPERTY(Transient)
    TObjectPtr<AAIController> AIOwner;
    UPROPERTY()
    TObjectPtr<UBehaviorTreeComponent> OwnerComponent;

    UFUNCTION()
	void OnPlayMontageEnded(FName NotifyName);
};
