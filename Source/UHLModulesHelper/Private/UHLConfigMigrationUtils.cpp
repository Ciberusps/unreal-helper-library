#pragma once

#include "UHLConfigMigrationUtils.h"

namespace UHLConfigMigrationUtils
{
	bool MigrateConfigSectionIfNeeded(
		const FString& FromSection, const FString& ToSection,
		const FString& MigrationKey,
		const FString& FromConfigFile, const FString& ToConfigFile)
	{
		// Check if migration has already been done.
		bool bMigrationDone = false;
		GConfig->GetBool(*ToSection, *MigrationKey, bMigrationDone, ToConfigFile);
		if (bMigrationDone)
		{
			UE_LOG(LogTemp, Log, TEXT("Migration already completed."));
			return false;
		}

		// Retrieve all lines from the old section.
		TArray<FString> SectionLines;
		if (GConfig->GetSection(*FromSection, SectionLines, FromConfigFile))
		{
			UE_LOG(LogTemp, Log, TEXT("Found %d lines in the old section."), SectionLines.Num());
			for (const FString& Line : SectionLines)
			{
				FString Key, Value;
				// Split the line into Key and Value.
				if (Line.Split(TEXT("="), &Key, &Value))
				{
					Key = Key.TrimStartAndEnd();
					Value = Value.TrimStartAndEnd();
					UE_LOG(LogTemp, Log, TEXT("Migrating key: %s with value: %s"), *Key, *Value);
					GConfig->AddToSection(*ToSection, *Key, *Value, ToConfigFile);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to split line: %s"), *Line);
				}
			}

			// Mark the migration as done.
			GConfig->SetBool(*ToSection, *MigrationKey, true, ToConfigFile);
			// Flush all changes to disk.
			GConfig->Flush(true, ToConfigFile);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Old settings section %s not found in %s"), *FromSection, *FromConfigFile);
		}

		return true;
	}
}
