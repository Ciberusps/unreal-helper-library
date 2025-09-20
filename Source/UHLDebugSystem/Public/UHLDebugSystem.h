// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"


class FUHLDebugSystemModule final : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void MigrateOldSettingsFromMainModule();
};
