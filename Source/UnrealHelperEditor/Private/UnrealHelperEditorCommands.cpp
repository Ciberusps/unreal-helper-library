// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealHelperEditorCommands.h"

#define LOCTEXT_NAMESPACE "FUnrealHelperEditorModule"

void FUnrealHelperEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UnrealHelperEditor", "Execute UnrealHelperEditor action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
