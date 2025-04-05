// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebug.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"
#include "UHLConfigMigrationUtils.h"
#include "Development/UHLDebugSubsystemSettings.h"

#define LOCTEXT_NAMESPACE "FUHLDebugModule"

void FUHLDebugModule::StartupModule()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TagsManager.AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UnrealHelperLibrary/Config/Tags"));

	MigrateOldSettingsFromMainModule();
}

void FUHLDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FUHLDebugModule::MigrateOldSettingsFromMainModule()
{
	UHLConfigMigrationUtils::MigrateConfigSectionIfNeeded(
		TEXT("/Script/UnrealHelperLibrary.UHLDebugSubsystemSettings"),
		TEXT("/Script/UHLDebug.UHLDebugSubsystemSettings"),
		TEXT("bMigrationFromOldSettingsDone"),
		GGameIni,
		GGameIni
	);

    UUHLDebugSubsystemSettings* Settings = GetMutableDefault<UUHLDebugSubsystemSettings>();
    if (Settings)
    {
        Settings->ReloadConfig();
    	// Settings->SaveConfig();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLDebugModule, UHLDebug)
