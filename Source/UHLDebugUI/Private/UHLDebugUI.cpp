// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugUI.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FUHLDebugUIModule"


void FUHLDebugUIModule::StartupModule()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TagsManager.AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UnrealHelperLibrary/Config/Tags"));
}

void FUHLDebugUIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLDebugUIModule, UHLDebugUI)
