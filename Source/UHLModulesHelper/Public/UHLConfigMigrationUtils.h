#pragma once

namespace UHLConfigMigrationUtils
{
	/**
	 * Utility to migrate all config keys from one section to another.
	 * WARNING migration will be done only when you change your settings
	 *
	 * @param FromSection config section from which your config will be migrated e.g. TEXT("/Script/UnrealHelperLibrary.UHLDebugSubsystemSettings")
	 * @param ToSection config section to which your config will be migrated e.g. TEXT("/Script/UHLDebug.UHLDebugSubsystemSettings")
	 * @param MigrationKey property in your class that will be used as marker that migration completed, required to create UPROPERTY(config) for this
	 * @param FromConfigFile config file that will be used to get config section
	 * @param ToConfigFile config file that will be used to set config section
	 * @return bool if successfully migrated
	 */
	UHLMODULESHELPER_API bool MigrateConfigSectionIfNeeded(
		const FString& FromSection,
		const FString& ToSection,
		const FString& MigrationKey,
		const FString& FromConfigFile,
		const FString& ToConfigFile
	);
}
