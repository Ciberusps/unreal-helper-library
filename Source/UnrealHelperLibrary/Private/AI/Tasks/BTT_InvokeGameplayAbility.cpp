// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_InvokeGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "UnrealHelperLibraryBPLibrary.h"

UBTT_InvokeGameplayAbility::UBTT_InvokeGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    NodeName = "InvokeGameplayAbility";
}

EBTNodeResult::Type UBTT_InvokeGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;

    AIOwner = OwnerComp.GetAIOwner();
    OwnerComponent = &OwnerComp;

    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(AIOwner->GetPawn());
    if (!AbilitySystemInterface)
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] OwnerActor \"%s\" don't have AbilitySystem(implements IAbilitySystemInterface) add it"), *AIOwner->GetPawn()->GetName()));
        }
        return EBTNodeResult::Failed;
    }

    ASC = AbilitySystemInterface->GetAbilitySystemComponent();
    TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
    ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), MatchingGameplayAbilities);

    AbilitySpec = MatchingGameplayAbilities[0];
    if (AbilitySpec != nullptr)
    {
        if (bWaitForFinishing)
        {
            ASC->OnAbilityEnded.AddUObject(this, &UBTT_InvokeGameplayAbility::OnAbilityEnded);
        }
        bool bAbilityActivated = ASC->TryActivateAbility(AbilitySpec->Handle, true);

        Result = bAbilityActivated ? EBTNodeResult::InProgress : EBTNodeResult::Failed;

        if (!bWaitForFinishing)
        {
            Result = EBTNodeResult::Succeeded;
        }

        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] TryActivateAbility - \"%s\" - %s"), *GameplayTag.ToString(), bAbilityActivated ? TEXT("activated") : TEXT("failed")));
        }
    }
    else
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Ability - \"%s\" - not found, give it to character if forgot"), *GameplayTag.ToString()));
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
        if (bWaitForFinishing)
        {
            ASC->OnAbilityEnded.RemoveAll(this);
        }

        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Task was aborted, CancelAbility - %s"), *GameplayTag.ToString()));
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

    const EBTNodeResult::Type NodeResult(EBTNodeResult::Succeeded);

    if (OwnerComponent.IsValid() && !bIsAborting)
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Ability ended - %s"), *GameplayTag.ToString()));
        }
        FinishLatentTask(*OwnerComponent, NodeResult);
    }
    if (bWaitForFinishing)
    {
        ASC->OnAbilityEnded.RemoveAll(this);
    }
}
