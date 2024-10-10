// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Composite/BTC_RandomSelector.h"

#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTC_RandomSelector)

UBTC_RandomSelector::UBTC_RandomSelector(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NodeName = "RandomSelector";
}

void UBTC_RandomSelector::RemoveUnusedChances()
{
    if (ChancesArray.Num() > GetChildrenNum())
    {
        int32 CountToRemove = ChancesArray.Num() - GetChildrenNum();
        ChancesArray.RemoveAt(ChancesArray.Num() - CountToRemove, CountToRemove);
    }
}

int32 UBTC_RandomSelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
    // success = quit
    int32 NextChildIdx = BTSpecialChild::ReturnToParent;

    if (PrevChild == BTSpecialChild::NotInitialized)
    {
        // newly activated: start from first
        NextChildIdx = GetRandomChildIdx();
    }
    else if (LastResult == EBTNodeResult::Failed && (PrevChild + 1) < GetChildrenNum())
    {
        // failed = choose next child
        NextChildIdx = GetRandomChildIdx();
    }

    return NextChildIdx;
}

FString UBTC_RandomSelector::GetStaticDescription() const
{
    // Chances Array
    FString Result = "";

    float ChancesSum = 0.0f;
    FString ChancesString = "";
    for (int32 i = 0; i < Children.Num(); i++)
    {
        FBTCompositeChild ChildNode = Children[i];

        FString Name = "";
        if (ChildNode.ChildComposite)
        {
            Name = ChildNode.ChildComposite->NodeName;
        }
        else if (ChildNode.ChildTask)
        {
            Name = ChildNode.ChildTask->NodeName;
        }
        if (!ChancesArray.IsValidIndex(i)) continue;

        float CurrentChance = ChancesArray[i];
        ChancesSum += CurrentChance;
        Result += FString::Printf(TEXT("- %s: %.2f \n"), *Name, CurrentChance);
    }

    FString SumWarning = ChancesSum != 1.0f ? FString(TEXT("⚠️")) : FString();
    Result = FString::Printf(TEXT("%s \n\nSum: %.2f %s \nChances: \n%s"), *Super::GetStaticDescription(), ChancesSum, *SumWarning, *Result);
    Result += GetErrorOrWarning();

    return Result;
}

#if WITH_EDITOR
FName UBTC_RandomSelector::GetNodeIconName() const
{
    return IsValidSelector() ? FName("BTEditor.Graph.BTNode.Composite.Selector.Icon") : FName("Graph.ConnectorFeedback.Error");
}
#endif

#if UE_VERSION_NEWER_THAN(5, 4, 0)
uint16 UBTC_RandomSelector::GetInstanceMemorySize() const
{
    return sizeof(FBTRandomSelectorMemory);
}

void UBTC_RandomSelector::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                   EBTMemoryInit::Type InitType) const
{
    InitializeNodeMemory<FBTRandomSelectorMemory>(NodeMemory, InitType);
}

void UBTC_RandomSelector::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
    EBTMemoryClear::Type CleanupType) const
{
    CleanupNodeMemory<FBTRandomSelectorMemory>(NodeMemory, CleanupType);
}
#endif

FString UBTC_RandomSelector::GetErrorOrWarning() const
{
    if (!IsValidSelector())
    {
        if (GetChildrenNum() > ChancesArray.Num())
        {
            return FString::Printf(TEXT("\n                    ❌ERROR❌ \nAdd chances for rest child nodes"));
        }
        // if RandomSelector have only one child, unreal somehow can crash randomly in different places Anims/AI/random components
        if (Children.Num() <= 1)
        {
            return FString::Printf(TEXT("\n                                                      ❌ERROR❌ \nUse with at least two childs, one child can lead to random crash in child nodes"));
        }
    }

    if (GetChildrenNum() < ChancesArray.Num())
    {
        return FString::Printf(TEXT("\n                     ⚠️WARNING⚠️ \nNot all chances have their child nodes"));
    }

    return FString();
}

int32 UBTC_RandomSelector::GetRandomChildIdx() const
{
    int32 ResultIdx = BTSpecialChild::ReturnToParent;

    TArray<FFloatRange> ChancesRanges = {};

    float SumChances = 0.0f;
    for (int32 i = 0; i < GetChildrenNum(); i++)
    {
        float MinValue = SumChances;

        float Chance = ChancesArray.IsValidIndex(i) ? ChancesArray[i] : 999999.0f;
        SumChances += Chance;

        FFloatRange ChanceRange = FFloatRange(MinValue, SumChances);
        ChancesRanges.Add(ChanceRange);
    }

    float RandomFloat = UKismetMathLibrary::RandomFloatInRange(0.0f, SumChances);

    for (FFloatRange ChanceRange : ChancesRanges)
    {
        if (ChanceRange.Contains(RandomFloat))
        {
            ResultIdx = ChancesRanges.IndexOfByKey(ChanceRange);
        }
    }

    return ResultIdx;
}
