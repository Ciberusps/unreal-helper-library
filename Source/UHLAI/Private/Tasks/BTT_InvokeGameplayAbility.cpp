// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/BTT_InvokeGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "AbilitySystemGlobals.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_InvokeGameplayAbility)

UBTT_InvokeGameplayAbility::UBTT_InvokeGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    NodeName = "InvokeGameplayAbility";

	bNotifyTaskFinished = true;
    // TODO probably instancing required or won't work with multiple InvokeGameplayAbilities/multiple enemies
    // as in PlayAnimMontage - yep its required
    // TODO try to remove instancing, to reproduce issue with stucking on "InvokeGameplayAbility"
    // create room with enemy only attacking infinitely
    // bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_InvokeGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;
    bool GameplayAbilitySpecFound = false;

    TObjectPtr<AAIController> AIOwner = OwnerComp.GetAIOwner();
    if (!AIOwner) return EBTNodeResult::Failed;

    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIOwner->GetPawn());
    if (!ASC)
    {
        if (bDebugMessages.GetValue(OwnerComp))
        {
            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] OwnerActor \"%s\" don't have AbilitySystem(implements IAbilitySystemInterface) add it"), *AIOwner->GetPawn()->GetName()));
        }
        return EBTNodeResult::Failed;
    }

	FGameplayAbilitySpec* AbilitySpec = nullptr;
	FGameplayAbilitySpecHandle* GameplayAbilitiesSpecHandle = nullptr;
	
    // TODO UHL->FindAbilitySpecByTags?
    TArray<FGameplayAbilitySpecHandle> GameplayAbilitiesSpecs = {};
    ASC->GetAllAbilities(GameplayAbilitiesSpecs);
    for (FGameplayAbilitySpecHandle GameplayAbilitiesSpecSearch : GameplayAbilitiesSpecs)
    {
        FGameplayAbilitySpec* AbilitySpecSearch = ASC->FindAbilitySpecFromHandle(GameplayAbilitiesSpecSearch);
        if (AbilitySpecSearch->Ability->GetAssetTags().HasAny(GameplayTag.GetValue(OwnerComp).GetSingleTagContainer()))
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
	        if (bWaitForFinishing.GetValue(OwnerComp))
	        {
	            ASC->OnAbilityEnded.AddUObject(this, &UBTT_InvokeGameplayAbility::OnAbilityEnded, &OwnerComp);
	        }
	        bool bAbilityActivated = ASC->TryActivateAbility(AbilitySpec->Handle, true);

	        Result = bAbilityActivated ? EBTNodeResult::InProgress : EBTNodeResult::Failed;

	        if (!bWaitForFinishing.GetValue(OwnerComp))
	        {
	            Result = EBTNodeResult::Succeeded;
	        }

	        if (bDebugMessages.GetValue(OwnerComp))
	        {
	            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] TryActivateAbility - \"%s\" - %s"), *GameplayTag.GetValue(OwnerComp).ToString(), bAbilityActivated ? TEXT("activated") : TEXT("failed")));
	        }
	    }
	    else
	    {
	        if (bDebugMessages.GetValue(OwnerComp))
	        {
	            UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Ability - \"%s\" - not found, give it to character if forgot"), *GameplayTag.GetValue(OwnerComp).ToString()));
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

	TObjectPtr<AAIController> AIOwner = OwnerComp.GetAIOwner();
	if (AIOwner)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIOwner->GetPawn());
		
		if (ASC)
		{
			const FGameplayTagContainer TagsContainer = FGameplayTagContainer(GameplayTag.GetValue(OwnerComp));
			ASC->CancelAbilities(&TagsContainer);
			if (bWaitForFinishing.GetValue(OwnerComp))
			{
				ASC->OnAbilityEnded.RemoveAll(this);
			}
	
			if (bDebugMessages.GetValue(OwnerComp))
			{
				UUnrealHelperLibraryBPL::DebugPrintStrings(FString::Printf(TEXT("[BTT_InvokeGameplayAbility] Task was aborted, CancelAbility - %s"), *GameplayTag.GetValue(OwnerComp).ToString()));
			}
		}
	}

    return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_InvokeGameplayAbility::OnTaskFinished(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FInvokeGameplayAbilityMemory* MyMemory = CastInstanceNodeMemory<FInvokeGameplayAbilityMemory>(NodeMemory);
	check(MyMemory);

	if (TaskResult != EBTNodeResult::InProgress && MyMemory->AbilityEndHandle.IsValid())
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		if (IsValid(AIController))
		{
			UAbilitySystemComponent* AbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIController->GetPawn());
			if (IsValid(AbilityComponent))
			{
				AbilityComponent->OnAbilityEnded.Remove(MyMemory->AbilityEndHandle);
				MyMemory->AbilityEndHandle.Reset();
			}
		}
	}
}

void UBTT_InvokeGameplayAbility::InitializeMemory(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	FInvokeGameplayAbilityMemory* MyMemory = CastInstanceNodeMemory<FInvokeGameplayAbilityMemory>(NodeMemory);
	check(MyMemory);
	
	MyMemory->AbilityEndHandle.Reset();
}

FString UBTT_InvokeGameplayAbility::GetStaticDescription() const
{
    return FString::Printf(TEXT("%s: \n%s"), *Super::GetStaticDescription(), !GameplayTag.ToString().IsEmpty() ? *GameplayTag.ToString() : TEXT(""));
}

void UBTT_InvokeGameplayAbility::OnAbilityEnded(
	const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* OwnerComp)
{
    // if not works check "AbilitySystemComponentTests.IsSameAbility"
    if (!AbilityEndedData.AbilityThatEnded->GetAssetTags().HasAllExact(FGameplayTagContainer(GameplayTag.GetValue(OwnerComp)))) return;

    const EBTNodeResult::Type NodeResult(EBTNodeResult::Succeeded);

	const AAIController* AIController = OwnerComp->GetAIOwner();
	const AActor* BehaviorOwner = IsValid(AIController) ? AIController->GetPawn() : nullptr;
	const AActor* AbilityOwner = AbilityEndedData.AbilityThatEnded->GetAvatarActorFromActorInfo();

	if (AbilityOwner == BehaviorOwner)
	{
		const EBTNodeResult::Type CancelResult = bTreatCancelledAbilityAsSuccess.GetValue(OwnerComp) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		const EBTNodeResult::Type TaskResult = AbilityEndedData.bWasCancelled ? CancelResult : EBTNodeResult::Succeeded;
		FinishLatentTask(*OwnerComp, TaskResult);
	}
}
