// Copyright (c) 2024 Pavel Penkov


#include "AI/Decorators/BTD_CheckGASGameplayTagsOnActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTD_CheckGASGameplayTagsOnActor)

UBTD_CheckGASGameplayTagsOnActor::UBTD_CheckGASGameplayTagsOnActor(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	NodeName = "GAS Gameplay Tag Condition";
    INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	// Accept only actors
	ActorToCheck.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_CheckGASGameplayTagsOnActor, ActorToCheck), AActor::StaticClass());

	// Default to using Self Actor
	ActorToCheck.SelectedKeyName = FBlackboard::KeySelf;

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

bool UBTD_CheckGASGameplayTagsOnActor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorToCheck.GetSelectedKeyID()));
	if (AbilitySystemInterface == nullptr)
	{
		return false;
	}
    UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
    if (!IsValid(ASC)) return false;

	switch (TagsToMatch)
	{
		case EGameplayContainerMatchType::All:
			return ASC->HasAllMatchingGameplayTags(GameplayTags);

		case EGameplayContainerMatchType::Any:
			return ASC->HasAnyMatchingGameplayTags(GameplayTags);

		default:
		{
			UE_LOG(LogBehaviorTree, Warning, TEXT("Invalid value for TagsToMatch (EGameplayContainerMatchType) %d.  Should only be Any or All."), static_cast<int32>(TagsToMatch));
			return false;
		}
	}
}

void UBTD_CheckGASGameplayTagsOnActor::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTD_CheckGASGameplayTagsOnActor::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *CachedDescription);
}

#if WITH_EDITOR
void UBTD_CheckGASGameplayTagsOnActor::BuildDescription()
{
	CachedDescription = TEXT("\n") + GameplayTags.ToMatchingText(TagsToMatch, IsInversed()).ToString()
        .Replace(TEXT("),"), TEXT("\n - "))
	    // .Replace(TEXT(": "), TEXT(""))
	    .Replace(TEXT("=("), TEXT("\n - "))
	    .Replace(TEXT("(GameplayTags"), TEXT(""))
	    .Replace(TEXT("(TagName="), TEXT(""))
	    .Replace(TEXT(")))"), TEXT(""))
	    .Replace(TEXT("\""), TEXT(""))
	    .Replace(TEXT("=)"), TEXT(""));
    // CachedDescription = FString::Printf(TEXT(" %s"), *GameplayTags.ToStringSimple(true));
}

void UBTD_CheckGASGameplayTagsOnActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property == NULL)
	{
		return;
	}

	BuildDescription();
}
#endif

void UBTD_CheckGASGameplayTagsOnActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		ActorToCheck.ResolveSelectedKey(*BBAsset);
	}

#if WITH_EDITOR
	BuildDescription();
#endif	// WITH_EDITOR
}

void UBTD_CheckGASGameplayTagsOnActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    RequestAbort(OwnerComp, EvaluateAbortType(OwnerComp));
}

