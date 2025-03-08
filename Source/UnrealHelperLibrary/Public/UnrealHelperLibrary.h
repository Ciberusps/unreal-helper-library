// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

UNREALHELPERLIBRARY_API DECLARE_LOG_CATEGORY_EXTERN(LogUnrealHelperLibrary, Log, All);
UNREALHELPERLIBRARY_API DECLARE_LOG_CATEGORY_EXTERN(LogUHLAbilitySystem, Log, All);

class FUnrealHelperLibraryModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
