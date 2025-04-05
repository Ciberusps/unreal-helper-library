// Pavel Penkov 2025 All Rights Reserved.

#include "UHLModulesHelper.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FUHLModulesHelperModule"


void FUHLModulesHelperModule::StartupModule()
{
}

void FUHLModulesHelperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLModulesHelperModule, UHLModulesHelper)
