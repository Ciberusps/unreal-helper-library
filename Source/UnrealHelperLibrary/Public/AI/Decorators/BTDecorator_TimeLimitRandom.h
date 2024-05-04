// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_TimeLimit.h"
#include "BTDecorator_TimeLimitRandom.generated.h"

/**
 * 
 */
UCLASS(hidecategories=Decorator)
class UNREALHELPERLIBRARY_API UBTDecorator_TimeLimitRandom : public UBTDecorator_TimeLimit
{
	GENERATED_BODY()
	
public:
	UBTDecorator_TimeLimitRandom(const FObjectInitializer& ObjectInitializer);

	/** TimeLimit */
	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	FFloatInterval TimeLimitRange = FFloatInterval(3.0f, 5.0f);
	UPROPERTY(EditAnywhere)
	bool bUseIntegers = true;

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
};
