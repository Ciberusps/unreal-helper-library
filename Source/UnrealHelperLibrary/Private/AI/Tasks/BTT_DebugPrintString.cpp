// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_DebugPrintString.h"

#include "UnrealHelperLibraryBPLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
        Color, Duration
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
    for (FString PrintString : PrintStrings)
    {
        Result.Append(PrintString);
    }
    return Result;
}
