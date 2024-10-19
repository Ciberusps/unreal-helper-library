// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "BTD_Base.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckGASGameplayTagsOnActor.generated.h"

/**
 * Actor should implement "IAbilitySystemInterface" to get AbilitySystemComponent
 */
UCLASS(Category = "UnrealHelperLibrary", DisplayName="Check GAS Gameplay Tags On Actor")
class UNREALHELPERLIBRARY_API UBTD_CheckGASGameplayTagsOnActor : public UBTD_Base
{
	GENERATED_BODY()

public:
    UBTD_CheckGASGameplayTagsOnActor(const FObjectInitializer& ObjectInitializer);

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
    virtual FString GetStaticDescription() const override;

protected:
    UPROPERTY(EditAnywhere, Category="GameplayTagCheck", Meta=(ToolTips="Which Actor (from the blackboard) should be checked for these GAS gameplay tags?"))
    struct FBlackboardKeySelector ActorToCheck;

    UPROPERTY(EditAnywhere, Category="GameplayTagCheck")
    EGameplayContainerMatchType TagsToMatch;

    UPROPERTY(EditAnywhere, Category="GameplayTagCheck")
    FGameplayTagContainer GameplayTags;

    /** cached description */
    UPROPERTY()
    FString CachedDescription;

#if WITH_EDITOR
    /** describe decorator and cache it */
    virtual void BuildDescription();

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
