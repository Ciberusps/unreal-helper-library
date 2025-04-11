// Pavel Penkov 2025 All Rights Reserved.

#include "UHLEditorCommands.h"

#define LOCTEXT_NAMESPACE "FUHLEditorModule"

void FUHLEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UHLEditorCommands", "Execute UHLEditorCommands action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
