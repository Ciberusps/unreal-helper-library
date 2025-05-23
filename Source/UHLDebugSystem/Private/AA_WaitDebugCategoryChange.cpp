// Pavel Penkov 2025 All Rights Reserved.


#include "AA_WaitDebugCategoryChange.h"

#include "UnrealEngine.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UHLDebugSystemSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AA_WaitDebugCategoryChange)

UAA_WaitDebugCategoryChange* UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(const UObject* WorldContext, FGameplayTag DebugCategoryTag, bool bCheckOnStart)
{
    // We must have a valid contextual world for this action, so we don't even make it
    // unless we can resolve the UWorld from WorldContext.
    UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
    if(!IsValid(ContextWorld) || !ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
    {
        return nullptr;
    }

    UAA_WaitDebugCategoryChange* NewAction = NewObject<UAA_WaitDebugCategoryChange>();
    NewAction->ContextWorld = ContextWorld;
    NewAction->DebugCategoryTagInternal = DebugCategoryTag;
    NewAction->bCheckOnStart = bCheckOnStart;
    NewAction->RegisterWithGameInstance(ContextWorld->GetGameInstance());
    return NewAction;
}

void UAA_WaitDebugCategoryChange::Activate()
{
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    if (IsValid(GameInstance))
    {
        UUHLDebugSystemSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSystemSubsystem>();
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
