// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/AA_WaitDebugCategoryChange.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"

UAA_WaitDebugCategoryChange* UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(const UObject* WorldContext, FGameplayTag DebugCategoryTag, bool bCheckOnStart)
{
    // We must have a valid contextual world for this action, so we don't even make it
    // unless we can resolve the UWorld from WorldContext.
    UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
    // if(!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
    // {
    //     return nullptr;
    // }

    // Create a new UMyDelayAsyncAction, and store function arguments in it.
    UAA_WaitDebugCategoryChange* NewAction = NewObject<UAA_WaitDebugCategoryChange>();
    NewAction->ContextWorld = ContextWorld;
    NewAction->DebugCategoryTagInternal = DebugCategoryTag;
    NewAction->bCheckOnStart = bCheckOnStart;
    // NewAction->RegisterWithGameInstance(ContextWorld->GetGameInstance());
    return NewAction;
}

void UAA_WaitDebugCategoryChange::Activate()
{
    // UWorld* World = GEngine->GetWorldFromContextObject(ContextWorld.Get(), EGetWorldErrorMode::ReturnNull);
    //
    // if (World == nullptr && GEditor != nullptr)
    // {
    //     World = GEditor->GetEditorWorldContext().World();
    // }

    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    if (IsValid(GameInstance))
    {
        UUHLDebugSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSubsystem>();
        UHLDebugSubsystem->OnDebugCategoryChanged.AddUniqueDynamic(this, &UAA_WaitDebugCategoryChange::OnDebugCategoryChange);
        if (bCheckOnStart)
        {
            OnDebugCategoryChange(DebugCategoryTagInternal, UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryTagInternal));
        }
    }
}

void UAA_WaitDebugCategoryChange::OnDebugCategoryChange(FGameplayTag DebugCategoryTag, bool bEnabled)
{
    if (DebugCategoryTag != DebugCategoryTagInternal) return;

    if (OnChange.IsBound())
    {
        OnChange.Broadcast(bEnabled);
    }

    if (bEnabled)
    {
        if (OnEnabled.IsBound())
        {
            OnEnabled.Broadcast(bEnabled);
        }
    }
    else
    {
        if (OnDisabled.IsBound())
        {
            OnDisabled.Broadcast(bEnabled);
        }
    }
}
