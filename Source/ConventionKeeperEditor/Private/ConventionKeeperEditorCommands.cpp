// Pavel Penkov 2025 All Rights Reserved.

#include "ConventionKeeperEditorCommands.h"

#define LOCTEXT_NAMESPACE "FUHLConventionKeeperEditorModule"

void FConventionKeeperEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UnrealHelperEditor", "Execute UnrealHelperEditor action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
