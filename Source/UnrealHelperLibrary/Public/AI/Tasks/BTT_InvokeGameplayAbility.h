// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_InvokeGameplayAbility.generated.h"

class IAbilitySystemInterface;

/**
 * TODO Invoke by Class option
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTT_InvokeGameplayAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_InvokeGameplayAbility(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(Category="Blackboard", EditAnywhere)
    FGameplayTag GameplayTag;
    UPROPERTY(Category="Blackboard", EditAnywhere)
    bool bActivate = true;
    UPROPERTY(Category="Blackboard", EditAnywhere)
    bool bWaitForFinishing = true;
    UPROPERTY(Category="Blackboard", EditAnywhere)
    bool bDebugMessages = false;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual FString GetStaticDescription() const override;

private:
    bool bIsAborting = false;
    FGameplayAbilitySpec* AbilitySpec = nullptr;
    FGameplayAbilitySpecHandle* GameplayAbilitiesSpec = nullptr;
    TWeakObjectPtr<UAbilitySystemComponent> ASC;
    /** Cached AIController owner of BehaviorTreeComponent. */
    UPROPERTY(Transient)
    TObjectPtr<AAIController> AIOwner;
    TWeakObjectPtr<UBehaviorTreeComponent> OwnerComponent;

    UFUNCTION()
    void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);
};
