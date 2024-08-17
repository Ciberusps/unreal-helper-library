// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealHelperEditor.h"
#include "UnrealHelperEditorStyle.h"
#include "UnrealHelperEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "UHEBlueprintThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

static const FName UnrealHelperEditorTabName("UnrealHelperEditor");

#define LOCTEXT_NAMESPACE "FUnrealHelperEditorModule"

void FUnrealHelperEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FUnrealHelperEditorStyle::Initialize();
	FUnrealHelperEditorStyle::ReloadTextures();

	FUnrealHelperEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUnrealHelperEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUnrealHelperEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUnrealHelperEditorModule::RegisterMenus));

    UThumbnailManager::Get().UnregisterCustomRenderer(UBlueprint::StaticClass());
    UThumbnailManager::Get().RegisterCustomRenderer(UBlueprint::StaticClass(), UUHEBlueprintThumbnailRenderer::StaticClass());
}

void FUnrealHelperEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FUnrealHelperEditorStyle::Shutdown();

	FUnrealHelperEditorCommands::Unregister();
}

void FUnrealHelperEditorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FUnrealHelperEditorModule::PluginButtonClicked()")),
							FText::FromString(TEXT("UnrealHelperEditor.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FUnrealHelperEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FUnrealHelperEditorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUnrealHelperEditorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealHelperEditorModule, UnrealHelperEditor)
