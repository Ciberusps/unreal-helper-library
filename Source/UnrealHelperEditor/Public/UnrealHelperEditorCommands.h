// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UnrealHelperEditorStyle.h"

class FUnrealHelperEditorCommands : public TCommands<FUnrealHelperEditorCommands>
{
public:

	FUnrealHelperEditorCommands()
		: TCommands<FUnrealHelperEditorCommands>(TEXT("UnrealHelperEditor"), NSLOCTEXT("Contexts", "UnrealHelperEditor", "UnrealHelperEditor Plugin"), NAME_None, FUnrealHelperEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
