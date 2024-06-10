// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_InvokeGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "UnrealHelperLibraryBPLibrary.h"

UBTT_InvokeGameplayAbility::UBTT_InvokeGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    NodeName = "InvokeGameplayAbility";
}

EBTNodeResult::Type UBTT_InvokeGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;

    AActor* OwnerActor = OwnerComp.GetOwner();
    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerActor);
    if (!AbilitySystemInterface)
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("OwnerActor \"%s\" don't have AbilitySystem(implements IAbilitySystemInterface) add it"), *OwnerActor->GetName()));
        }
        return EBTNodeResult::Failed;
    }

    ASC = AbilitySystemInterface->GetAbilitySystemComponent();
    TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
    ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), MatchingGameplayAbilities);

    AbilitySpec = MatchingGameplayAbilities[0];
    if (AbilitySpec != nullptr)
    {
        ASC->OnAbilityEnded.AddUObject(this, &UBTT_InvokeGameplayAbility::OnAbilityEnded);
        bool bAbilityActivated = ASC->TryActivateAbility(AbilitySpec->Handle, true);

        Result = bAbilityActivated ? EBTNodeResult::InProgress : EBTNodeResult::Failed;

        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("TryActivateAbility - \"%s\" - %hhd"), *GameplayTag.ToString(), bAbilityActivated));
        }
    }
    else
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("Ability - \"%s\" - not found, give it to character if forgot"), *GameplayTag.ToString()));
        }
    }

    return Result;
}

EBTNodeResult::Type UBTT_InvokeGameplayAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    bIsAborting = true;
    if (ASC.IsValid())
    {
        ASC->CancelAbilityHandle(AbilitySpec->Handle);
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("Task was aborted, CancelAbility - %s"), *GameplayTag.ToString()));
        }
    }
    return Super::AbortTask(OwnerComp, NodeMemory);
}

FString UBTT_InvokeGameplayAbility::GetStaticDescription() const
{
    return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), GameplayTag.IsValid() ? *GameplayTag.ToString() : TEXT(""));
}

void UBTT_InvokeGameplayAbility::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
    // if not works check "AbilitySystemComponentTests.IsSameAbility"
    if (AbilityEndedData.AbilitySpecHandle != AbilitySpec->Handle) return;

    UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
    const EBTNodeResult::Type NodeResult(EBTNodeResult::Succeeded);

    if (OwnerComp && !bIsAborting)
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("Ability ended - %s"), *GameplayTag.ToString()));
        }
        FinishLatentTask(*OwnerComp, NodeResult);
    }
}
