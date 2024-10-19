// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "Misc/EngineVersionComparison.h"
#include "BTC_RandomSelector.generated.h"

struct FBTRandomSelectorMemory
{
};

/**
 * RandomSelector composite node.
 * RandomSelector random between child nodes using chances, and will stop executing its children when one of their children succeeds.
 * If a Selector's child succeeds, the Selector succeeds. If all the Selector's children fail, the Selector fails.
 * P.S. space for improvement - memory, but not sure that its required,
 * just random until we get valid node is seems looks like is enough
 * https://www.youtube.com/watch?v=wI_kwwvc-pw&t=781s&ab_channel=VeryHotShark
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTC_RandomSelector : public UBTCompositeNode
{
	GENERATED_BODY()

public:
	explicit UBTC_RandomSelector(const FObjectInitializer& ObjectInitializer);

	// TODO validate that chances count == ChildrenNum
	// if no chance specified, node without chance will win always
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomSelector", meta=(ForceUnits="Multiplier"))
	TArray<float> ChancesArray = { 0.5f, 0.5f };

    UFUNCTION(BlueprintCallable, CallInEditor, Category="RandomSelector")
    void RemoveUnusedChances();

protected:
	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

#if UE_VERSION_NEWER_THAN(5, 4, 0)
    // 5.4.0 and up only code
    virtual uint16 GetInstanceMemorySize() const override;
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
#endif

private:
	UFUNCTION()
	bool IsValidSelector() const { return GetChildrenNum() > 1 && GetChildrenNum() <= ChancesArray.Num(); };
	UFUNCTION()
	FString GetErrorOrWarning() const;
	UFUNCTION()
	int32 GetRandomChildIdx() const;

};
