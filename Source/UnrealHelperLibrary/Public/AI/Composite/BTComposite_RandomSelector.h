// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BTComposite_RandomSelector.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UBTComposite_RandomSelector : public UBTCompositeNode
{
	GENERATED_BODY()

public:
	explicit UBTComposite_RandomSelector(const FObjectInitializer& ObjectInitializer);

	// if no chance specified, node without chance will win always
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta=(DeprecatedProperty))
	TMap<FString, float> Chances = {
		{ "1", 1.0f },
		{ "2", 1.0f },
		{ "3", 1.0f },
		{ "4", 1.0f },
		{ "5", 1.0f },
	};
	// TODO validate that chances count == ChildrenNum
	// if no chance specified, node without chance will win always
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ChancesArray = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

protected:
	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

private:
	UFUNCTION()
	bool IsValidSelector() const { return GetChildrenNum() <= ChancesArray.Num(); };
	UFUNCTION()
	FString GetErrorOrWarning() const;
	UFUNCTION()
	int32 GetRandomChildIdx() const;
	
};
