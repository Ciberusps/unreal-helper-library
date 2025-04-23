// Copyright (c) 2024 NextGenium

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Decorators/BTD_Base.h"
#include "BTD_GameplayEffectCooldown.generated.h"

struct FBTGameplayEffectCooldownDecoratorMemory
{
	double LastUseTimestamp;
	uint8 bRequestedRestart : 1;
};

/**
 * Actor should implement "IAbilitySystemInterface"
 */
UCLASS()
class UHLAI_API UBTD_GameplayEffectCooldown : public UBTD_Base
{
	GENERATED_BODY()

public:
	UBTD_GameplayEffectCooldown(const FObjectInitializer& ObjectInitializer);

	// Tag used in GameplayEffect to check cooldown
	UPROPERTY(Category="Decorator", EditAnywhere)
	FGameplayTag CooldownGameplayEffectTag;

	// TODO: make option to check cooldowns by this query
	// UPROPERTY(Category="Decorator", EditAnywhere)
	// FGameplayEffectQuery GameplayEffectQuery;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
