// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/AA_WaitDebugCategoryChange.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

UAA_WaitDebugCategoryChange* UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(const UObject* WorldContext, FGameplayTag DebugCategory)
{
    // This function is just a factory that creates a UMyDelayAsyncAction instance.

    // We must have a valid contextual world for this action, so we don't even make it
    // unless we can resolve the UWorld from WorldContext.
    UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
    if(!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
    {
        return nullptr;
    }

    // Create a new UMyDelayAsyncAction, and store function arguments in it.
    UAA_WaitDebugCategoryChange* NewAction = NewObject<UAA_WaitDebugCategoryChange>();
    NewAction->ContextWorld = ContextWorld;
    NewAction->DebugCategory = DebugCategory;
    NewAction->RegisterWithGameInstance(ContextWorld->GetGameInstance());
    return NewAction;
}

void UAA_WaitDebugCategoryChange::Activate()
{
    UUHLDebugSubsystem* UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSubsystem>();
    UHLDebugSubsystem->OnDebugCategoryChanged.AddUniqueDynamic(this, &UAA_WaitDebugCategoryChange::OnDebugCategoryChange);
}

void UAA_WaitDebugCategoryChange::Cancel()
{
    Super::Cancel();
}

void UAA_WaitDebugCategoryChange::OnDebugCategoryChange(FGameplayTag DebugCategoryTag, bool bEnabled)
{
    if (DebugCategoryTag != DebugCategory) return;

    if (OnChange.IsBound())
    {
        OnChange.Broadcast(DebugCategoryTag, bEnabled);
    }
    if (bEnabled)
    {
        if (OnEnabled.IsBound())
        {
            OnEnabled.Broadcast(DebugCategoryTag, bEnabled);
        }
    }
    else
    {
        if (OnDisabled.IsBound())
        {
            OnDisabled.Broadcast(DebugCategoryTag, bEnabled);
        }
    }
}
