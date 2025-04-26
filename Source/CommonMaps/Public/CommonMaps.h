// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCommonMapsModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void ExtendContextMenu();
	void AddToCommonMapsFromMenu(FName CategoryName);
	void CreateCategorySelectionSubmenu(FMenuBuilder& MenuBuilder);
};
