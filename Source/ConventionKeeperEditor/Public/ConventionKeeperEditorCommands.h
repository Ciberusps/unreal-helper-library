// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ConventionKeeperEditorStyle.h"

class FConventionKeeperEditorCommands : public TCommands<FConventionKeeperEditorCommands>
{
public:

	FConventionKeeperEditorCommands()
		: TCommands<FConventionKeeperEditorCommands>(TEXT("ConventionKeeperEditor"), NSLOCTEXT("Contexts", "ConventionKeeperEditor", "UHL Plugin"), NAME_None, FConventionKeeperEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
