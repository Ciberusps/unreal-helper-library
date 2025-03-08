// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DebugPrintString.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary")
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
    UPROPERTY(Category="Decorator", EditAnywhere)
    FName Key = NAME_None;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;

private:
    FString GetFinalString() const;
};
