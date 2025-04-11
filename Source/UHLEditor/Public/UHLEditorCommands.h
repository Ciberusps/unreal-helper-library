// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UHLEditorStyle.h"

class FUHLEditorCommands : public TCommands<FUHLEditorCommands>
{
public:

	FUHLEditorCommands()
		: TCommands<FUHLEditorCommands>(TEXT("UHLEditor"), NSLOCTEXT("Contexts", "UHLEditor", "UHLEditor Plugin"), NAME_None, FUHLEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
