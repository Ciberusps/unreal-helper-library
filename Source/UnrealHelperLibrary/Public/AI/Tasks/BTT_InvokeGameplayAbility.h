// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_InvokeGameplayAbility.generated.h"

class IAbilitySystemInterface;

/**
 * TODO Invoke by Class option?
 * 1) tbh its bad practice to have hard refs on classes,
 * no need in this options, better to force good practices - dont hard ref abilities
 * 2) it brokes good architechture/behaviorTrees reusability if we hard ref ability
 * it means that nested abilities can't be activated, with activation by tag
 * all enemies can have different abilities implementation using same tag
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
    // TODO move to Memory
    FGameplayAbilitySpec* AbilitySpec = nullptr;
    // TODO move to Memory
    FGameplayAbilitySpecHandle* GameplayAbilitiesSpecHandle = nullptr;
    // TODO move to Memory
    TWeakObjectPtr<UAbilitySystemComponent> ASC;
    /** Cached AIController owner of BehaviorTreeComponent. */

    TWeakObjectPtr<UBehaviorTreeComponent> OwnerComponent;

    UFUNCTION()
    void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);
};
