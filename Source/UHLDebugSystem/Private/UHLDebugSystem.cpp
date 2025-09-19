// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugSystem.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Development/UHLDebugSystemSettings.h"

#define LOCTEXT_NAMESPACE "FUHLDebugSystemModule"

void FUHLDebugSystemModule::StartupModule()
{
	MigrateOldSettingsFromMainModule();
}

void FUHLDebugSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FUHLDebugSystemModule::MigrateOldSettingsFromMainModule()
{
    bool bMigrationDone = false;
    GConfig->GetBool(TEXT("/Script/UHLDebugSystem.UHLDebugSystemSettings"), TEXT("bMigrationFromOldSettingsDone"), bMigrationDone, GGameIni);
    if (!bMigrationDone)
    {
        // Copy all keys from old section if it exists
        if (GConfig->DoesSectionExist(TEXT("/Script/UnrealHelperLibrary.UHLDebugSubsystemSettings"), GGameIni))
        {
            TArray<FString> Keys;
            GConfig->GetSection(TEXT("/Script/UnrealHelperLibrary.UHLDebugSubsystemSettings"), Keys, GGameIni);
            for (const FString& KeyLine : Keys)
            {
                FString Key, Value;
                if (KeyLine.Split(TEXT("="), &Key, &Value))
                {
                    GConfig->SetString(TEXT("/Script/UHLDebugSystem.UHLDebugSystemSettings"), *Key, *Value, GGameIni);
                }
            }
        }
        GConfig->SetBool(TEXT("/Script/UHLDebugSystem.UHLDebugSystemSettings"), TEXT("bMigrationFromOldSettingsDone"), true, GGameIni);
        GConfig->Flush(false, GGameIni);
    }

    UUHLDebugSystemSettings* Settings = GetMutableDefault<UUHLDebugSystemSettings>();
    if (Settings)
    {
        Settings->ReloadConfig();
    	// Settings->SaveConfig();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLDebugSystemModule, UHLDebugSystem)
