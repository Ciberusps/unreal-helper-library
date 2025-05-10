// Pavel Penkov 2025 All Rights Reserved.

#include "UHLStateTree.h"

#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FUHLStateTreeModule"


void FUHLStateTreeModule::StartupModule()
{
}

void FUHLStateTreeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLStateTreeModule, UHLStateTree)
