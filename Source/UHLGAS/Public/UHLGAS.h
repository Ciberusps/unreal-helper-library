// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

UHLGAS_API DECLARE_LOG_CATEGORY_EXTERN(LogUHLAbilitySystem, Log, All);

class FUHLGASModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
