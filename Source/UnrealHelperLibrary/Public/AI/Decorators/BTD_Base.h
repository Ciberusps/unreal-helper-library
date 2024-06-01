// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALHELPERLIBRARY_API UBTD_Base : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_Base(const FObjectInitializer& ObjectInitializer);
	
	FString GetPropertiesDetails() const;
	/** notify about changes in blackboard */
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	
protected:
	// BTDecorator_BlueprintBase
	enum class EAbortType : uint8
	{
		NoAbort,
		ActivateBranch,
		DeactivateBranch,
		Unknown,
	};
	/** gets set to true if decorator declared BB keys it can potentially observe */
	uint32 bIsObservingBB : 1;
	/** blackboard key names that should be observed */
	UPROPERTY()
	TArray<FName> ObservedKeyNames;
	/** properties with runtime values, stored only in class default object */
	TArray<FProperty*> PropertyData;
	/** show detailed information about properties */
	UPROPERTY(EditInstanceOnly, Category=Description)
	bool bShowPropertyDetails = true;
	/** return this decorator abort type in current circumstances */
	EAbortType EvaluateAbortType(UBehaviorTreeComponent& OwnerComp) const;
	void RequestAbort(UBehaviorTreeComponent& OwnerComp, const EAbortType Type);
	
	virtual void PostLoad() override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** initialize data about blueprint defined properties
	 * copy-paste from UBTDecorator_BlueprintBase
	 */
	virtual void InitializeProperties();

	/** setup node name
	 * copy-paste from UBTDecorator_BlueprintBase
	 */
	virtual void PostInitProperties() override;
	// ~BTDecorator_BlueprintBase
};
