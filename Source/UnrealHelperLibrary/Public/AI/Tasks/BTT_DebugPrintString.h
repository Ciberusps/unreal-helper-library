// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DebugPrintString.generated.h"

// UENUM()
// enum class EDebugPrintString_Strings
// {
//     A,
//     B,
//     C,
//     D UMETA(Hidden),
//     E,
//     F,
//     G,
//     H,
//     I,
//     J,
//     Num UMETA(Hidden)
// };

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UBTT_DebugPrintString : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTT_DebugPrintString(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(Category="Decorator", EditAnywhere, meta=(EditFixedSize, MultiLine))
    TArray<FString> PrintStrings = { "", "", "" };
    UPROPERTY(Category="Decorator", EditAnywhere)
    float Duration = 3.5f;
    UPROPERTY(Category="Decorator", EditAnywhere)
    FLinearColor Color = FLinearColor(0, 0.66, 1);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;

private:
    FString GetFinalString() const;
};
