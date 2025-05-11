// Pavel Penkov 2025 All Rights Reserved.

#include "UHLEditor.h"
#include "UHLEditorStyle.h"
#include "UHLEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "UHLEditorBlueprintThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

static const FName UHLDebugSystemEditorTabName("UHLEditor");

#define LOCTEXT_NAMESPACE "FUHLEditorModule"

void FUHLEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FUHLEditorStyle::Initialize();
	FUHLEditorStyle::ReloadTextures();

	FUHLEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUHLEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUHLEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUHLEditorModule::RegisterMenus));

    UThumbnailManager::Get().UnregisterCustomRenderer(UBlueprint::StaticClass());
    UThumbnailManager::Get().RegisterCustomRenderer(UBlueprint::StaticClass(), UUHLEditorBlueprintThumbnailRenderer::StaticClass());
}

void FUHLEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FUHLEditorStyle::Shutdown();

	FUHLEditorCommands::Unregister();
}

void FUHLEditorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FUHLEditorModule::PluginButtonClicked()")),
							FText::FromString(TEXT("UHLEditor.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FUHLEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FUHLEditorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUHLEditorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLEditorModule, UHLEditor)
