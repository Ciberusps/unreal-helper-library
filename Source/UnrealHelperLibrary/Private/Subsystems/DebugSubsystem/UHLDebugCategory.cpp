// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem/UHLDebugCategory.h"

#include "Subsystems/DebugSubsystem/UHLDebugCategoryComponent.h"


bool FUHLDebugCategory::TryActivate(UObject* ContextObj)
{
    bool bResult = false;

    int32 ComponentsActivated = 0;
    int32 ComponentsRequiredToActivate = Components.Num();
    for (TSubclassOf<UUHLDebugCategoryComponent> ComponentClass : Components)
    {
        if (!ComponentClass)
        {
            ComponentsRequiredToActivate--;
            continue;
        };

        UUHLDebugCategoryComponent* Component = GetOrCreateDebugCategoryComponent(ComponentClass, ContextObj);
        if (Component->CanActivate(ContextObj))
        {
            Component->Activate(ContextObj);
            ComponentsActivated++;
        }
    }
    if (ComponentsActivated == ComponentsRequiredToActivate)
    {
        bResult = true;
    }

    bEnabled = true;
    return bResult;
}

void FUHLDebugCategory::TryDeactivate(UObject* ContextObj)
{
    for (UUHLDebugCategoryComponent* InstancedComponent : InstancedComponents)
    {
        if (InstancedComponent)
        {
            InstancedComponent->Deactivate(ContextObj);
        }
    }
    bEnabled = false;
}

UUHLDebugCategoryComponent* FUHLDebugCategory::GetOrCreateDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj)
{
    UUHLDebugCategoryComponent* Component = nullptr;
    UUHLDebugCategoryComponent** FoundComponent = GetDebugCategoryComponent(ComponentClass, ContextObj);
    if (!FoundComponent)
    {
        Component = NewObject<UUHLDebugCategoryComponent>(ContextObj, ComponentClass);
        InstancedComponents.Add(Component);
    }
    else
    {
        Component = *FoundComponent;
    }
    return Component;
}

UUHLDebugCategoryComponent** FUHLDebugCategory::GetDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj)
{
    return InstancedComponents.FindByPredicate([=](UUHLDebugCategoryComponent* DebugCategoryComponent)
        {
            return DebugCategoryComponent->GetClass() == ComponentClass;
        });
}