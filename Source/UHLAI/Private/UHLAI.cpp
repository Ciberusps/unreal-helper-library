// Pavel Penkov 2025 All Rights Reserved.

#include "UHLAI.h"

#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FUHLAIModule"


void FUHLAIModule::StartupModule()
{
}

void FUHLAIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLAIModule, UHLAI)
