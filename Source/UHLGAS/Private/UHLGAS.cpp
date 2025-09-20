// Pavel Penkov 2025 All Rights Reserved.

#include "UHLGAS.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FUHLGASModule"

DEFINE_LOG_CATEGORY(LogUHLAbilitySystem);

void FUHLGASModule::StartupModule()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TagsManager.AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UnrealHelperLibrary/Config/Tags"));
}

void FUHLGASModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLGASModule, UHLGAS)
