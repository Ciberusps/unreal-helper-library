// Pavel Penkov 2025 All Rights Reserved.

#include "UHLAIEditor.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "UHLAIEditorValueOrBBKeyDetails.h"

static const FName UHLAIEditorTabName("UHLAIEditor");

#define LOCTEXT_NAMESPACE "FUHLAIEditorModule"

void FUHLAIEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// PropertyModule.RegisterCustomPropertyTypeLayout("ValueOrBBKey_GameplayTag", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FValueOrBBKeyDetails_GameplayTag::MakeInstance));
	// PropertyModule.RegisterCustomPropertyTypeLayout("ValueOrBBKey_GameplayTag", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FValueOrBBKeyDetails_GameplayTag::MakeInstance));
	// PropertyModule.RegisterCustomClassLayout("BlackboardKeyType_Class", FOnGetDetailCustomizationInstance::CreateStatic(&FBlackboardKeyDetails_Class::MakeInstance));
	// PropertyModule.RegisterCustomPropertyTypeLayout("ValueOrBBKey_GameplayTag", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FValueOrBBKeyDetails_Struct::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("ValueOrBBKey_GameplayTag", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FValueOrBBKeyDetails_GameplayTag::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FUHLAIEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Unregister the details customization
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout("ValueOrBBKey_GameplayTag");
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLAIEditorModule, UHLAIEditor)
