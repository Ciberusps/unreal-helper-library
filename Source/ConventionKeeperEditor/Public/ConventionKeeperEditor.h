// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConventionKeeper, Log, All);

class FToolBarBuilder;
class FMenuBuilder;

class FConventionKeeperEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/** ~IModuleInterface implementation */
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:
	void RegisterMenus();

	bool CheckAssetPathExists(const FString& PackagePath);


private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
