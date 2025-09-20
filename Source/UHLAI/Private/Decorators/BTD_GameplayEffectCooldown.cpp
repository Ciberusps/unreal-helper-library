// Copyright (c) 2024 NextGenium


#include "Decorators/BTD_GameplayEffectCooldown.h"

#include "Engine/World.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_GameplayEffectCooldown)

UBTD_GameplayEffectCooldown::UBTD_GameplayEffectCooldown(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "GameplayEffect Cooldown";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
	
	// aborting child nodes doesn't makes sense, cooldown starts after leaving this branch
	bAllowAbortChildNodes = false;
}

bool UBTD_GameplayEffectCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// FBTGameplayEffectCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FBTGameplayEffectCooldownDecoratorMemory>(NodeMemory);
	// const double RecalcTime = (OwnerComp.GetWorld()->GetTimeSeconds() - CoolDownTime.GetValue(OwnerComp));
	// return RecalcTime >= DecoratorMemory->LastUseTimestamp;

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	AActor* OwnerActor = OwnerController ? OwnerController->GetPawn() : nullptr;
	if (!OwnerActor) return true;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (!ASC) return true;

	FGameplayEffectQuery const ActiveGameplayEffectsQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(CooldownGameplayEffectTag));
	TArray<float> EffectsTimeRemaining = ASC->GetActiveEffectsTimeRemaining(ActiveGameplayEffectsQuery);
	if (EffectsTimeRemaining.Num() > 0)
	{
		return EffectsTimeRemaining[0] > 0.0f ? false : true; 
	}
	return true;
}

void UBTD_GameplayEffectCooldown::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	FBTGameplayEffectCooldownDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTGameplayEffectCooldownDecoratorMemory>(SearchData);
	DecoratorMemory->LastUseTimestamp = SearchData.OwnerComp.GetWorld()->GetTimeSeconds();
	DecoratorMemory->bRequestedRestart = false;
}

void UBTD_GameplayEffectCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// FBTGameplayEffectCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FBTGameplayEffectCooldownDecoratorMemory>(NodeMemory);
	// if (!DecoratorMemory->bRequestedRestart)
	// {
	// 	const double RecalcTime = (OwnerComp.GetWorld()->GetTimeSeconds() - CoolDownTime.GetValue(OwnerComp));
	// 	if (RecalcTime >= DecoratorMemory->LastUseTimestamp)
	// 	{
	// 		DecoratorMemory->bRequestedRestart = true;
	// 		OwnerComp.RequestExecution(this);
	// 	}
	// }
}

FString UBTD_GameplayEffectCooldown::GetStaticDescription() const
{
	return "";
	// // basic info: result after time
	// return FString::Printf(TEXT("%s: lock for %s s after execution and return %s"), *Super::GetStaticDescription(),
	// 	*CoolDownTime.ToString(), *UBehaviorTreeTypes::DescribeNodeResult(EBTNodeResult::Failed));
}

void UBTD_GameplayEffectCooldown::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	// FBTGameplayEffectCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FBTGameplayEffectCooldownDecoratorMemory>(NodeMemory);
	// const double TimePassed = OwnerComp.GetWorld()->GetTimeSeconds() - DecoratorMemory->LastUseTimestamp;
	//
	// if (TimePassed < CoolDownTime.GetValue(OwnerComp))
	// {
	// 	Values.Add(FString::Printf(TEXT("%s in %ss"),
	// 		(FlowAbortMode == EBTFlowAbortMode::None) ? TEXT("unlock") : TEXT("restart"),
	// 		*FString::SanitizeFloat(CoolDownTime.GetValue(OwnerComp) - TimePassed)));
	// }
}

uint16 UBTD_GameplayEffectCooldown::GetInstanceMemorySize() const
{
	return sizeof(FBTGameplayEffectCooldownDecoratorMemory);
}

void UBTD_GameplayEffectCooldown::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	FBTGameplayEffectCooldownDecoratorMemory* DecoratorMemory = InitializeNodeMemory<FBTGameplayEffectCooldownDecoratorMemory>(NodeMemory, InitType);
	if (InitType == EBTMemoryInit::Initialize)
	{
		DecoratorMemory->LastUseTimestamp = TNumericLimits<double>::Lowest();
	}

	DecoratorMemory->bRequestedRestart = false; ;
}

void UBTD_GameplayEffectCooldown::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTGameplayEffectCooldownDecoratorMemory>(NodeMemory, CleanupType);
}

#if WITH_EDITOR

FName UBTD_GameplayEffectCooldown::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.Cooldown.Icon");
}

#endif	// WITH_EDITOR

