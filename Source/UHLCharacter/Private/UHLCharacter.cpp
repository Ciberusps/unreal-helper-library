// Pavel Penkov 2025 All Rights Reserved.

#include "UHLCharacter.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FUHLCharacterModule"


void FUHLCharacterModule::StartupModule()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TagsManager.AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UnrealHelperLibrary/Config/Tags"));
}

void FUHLCharacterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLCharacterModule, UHLCharacter)
