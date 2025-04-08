// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugSystem.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"
#include "UHLConfigMigrationUtils.h"
#include "Development/UHLDebugSystemSettings.h"

#define LOCTEXT_NAMESPACE "FUHLDebugSystemModule"

void FUHLDebugSystemModule::StartupModule()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TagsManager.AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UnrealHelperLibrary/Config/Tags"));

	MigrateOldSettingsFromMainModule();
}

void FUHLDebugSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FUHLDebugSystemModule::MigrateOldSettingsFromMainModule()
{
	UHLConfigMigrationUtils::MigrateConfigSectionIfNeeded(
		TEXT("/Script/UnrealHelperLibrary.UHLDebugSubsystemSettings"),
		TEXT("/Script/UHLDebugSystem.UHLDebugSystemSubsystemSettings"),
		TEXT("bMigrationFromOldSettingsDone"),
		GGameIni,
		GGameIni
	);

    UUHLDebugSystemSettings* Settings = GetMutableDefault<UUHLDebugSystemSettings>();
    if (Settings)
    {
        Settings->ReloadConfig();
    	// Settings->SaveConfig();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLDebugSystemModule, UHLDebugSystem)
