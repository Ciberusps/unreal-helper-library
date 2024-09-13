// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/UHLDebugSubsystemSettings.h"

FName UUHLDebugSubsystemSettings::GetCategoryName() const
{
    return FApp::GetProjectName();
}

void UUHLDebugSubsystemSettings::PostInitProperties()
{
    Super::PostInitProperties();
    // for (const FUHLDebugCategory& DebugCategory : DebugCategories)
    // {
    //     if (!DebugCategories.Contains(DebugCategory.Tag))
    //     {
    //         DebugCategories.Add(DebugCategory, false);
    //     }
    // }
}
