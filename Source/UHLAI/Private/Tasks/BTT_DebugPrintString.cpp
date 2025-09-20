// Pavel Penkov 2025 All Rights Reserved.


#include "Tasks/BTT_DebugPrintString.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTT_DebugPrintString)

UBTT_DebugPrintString::UBTT_DebugPrintString(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "DebugPrintString";
}

EBTNodeResult::Type UBTT_DebugPrintString::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UKismetSystemLibrary::PrintString(
        nullptr, GetFinalString(),
        true, true,
        Color, Duration.GetValue(OwnerComp), Key.GetValue(OwnerComp)
    );
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UBTT_DebugPrintString::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: \"%s\""), *Super::GetStaticDescription(), *GetFinalString());
}

FString UBTT_DebugPrintString::GetFinalString() const
{
    FString Result = "";
    for (FValueOrBBKey_String PrintString : PrintStrings)
    {
        Result.Append(PrintString.ToString());
    }
    return Result;
}
