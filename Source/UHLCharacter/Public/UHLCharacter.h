// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"


class FUHLCharacterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
