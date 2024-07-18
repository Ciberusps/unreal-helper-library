// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_InvokeGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "Utils/UnrealHelperLibraryBPL.h"

UBTT_InvokeGameplayAbility::UBTT_InvokeGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    NodeName = "InvokeGameplayAbility";
}

EBTNodeResult::Type UBTT_InvokeGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;
    bool GameplayAbilitySpecFound = false;

    AIOwner = OwnerComp.GetAIOwner();
    OwnerComponent = &OwnerComp;

    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(AIOwner->GetPawn());
    if (!AbilitySystemInterface)
    {
        if (bDebugMessages)
        {
            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] OwnerActor \"%s\" don't have AbilitySystem(implements IAbilitySystemInterface) add it"), *AIOwner->GetPawn()->GetName()));
        }
        return EBTNodeResult::Failed;
    }

    ASC = AbilitySystemInterface->GetAbilitySystemComponent();

    // TODO UHL->FindAbilitySpecByTags?
    TArray<FGameplayAbilitySpecHandle> GameplayAbilitiesSpecs = {};
    ASC->GetAllAbilities(GameplayAbilitiesSpecs);
    for (FGameplayAbilitySpecHandle GameplayAbilitiesSpecSearch : GameplayAbilitiesSpecs)
    {
        FGameplayAbilitySpec* AbilitySpecSearch = ASC->FindAbilitySpecFromHandle(GameplayAbilitiesSpecSearch);
        if (AbilitySpecSearch->Ability->AbilityTags.HasAny(GameplayTag.GetSingleTagContainer()))
        {
            AbilitySpec = AbilitySpecSearch;
            GameplayAbilitiesSpecHandle = &GameplayAbilitiesSpecSearch;
            GameplayAbilitySpecFound = true;
            break;
        }
    }

    if (bActivate)
    {
	    if (GameplayAbilitySpecFound && AbilitySpec != nullptr)
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
	            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] TryActivateAbility - \"%s\" - %s"), *GameplayTag.ToString(), bAbilityActivated ? TEXT("activated") : TEXT("failed")));
	        }
	    }
	    else
	    {
	        if (bDebugMessages)
	        {
	            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Ability - \"%s\" - not found, give it to character if forgot"), *GameplayTag.ToString()));
	        }
	    }
    }
    else
    {
        if (GameplayAbilitySpecFound && GameplayAbilitiesSpecHandle != nullptr)
        {
            ASC->CancelAbilityHandle(*GameplayAbilitiesSpecHandle);
        }
        Result = EBTNodeResult::Succeeded;
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
            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Task was aborted, CancelAbility - %s"), *GameplayTag.ToString()));
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
            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Ability ended - %s"), *GameplayTag.ToString()));
        }
        FinishLatentTask(*OwnerComponent, NodeResult);
    }
    if (bWaitForFinishing)
    {
        ASC->OnAbilityEnded.RemoveAll(this);
    }
}
