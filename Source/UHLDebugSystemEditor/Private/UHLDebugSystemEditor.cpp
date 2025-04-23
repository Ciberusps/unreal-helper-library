// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugSystemEditor.h"

#include "FileHelpers.h"
#include "Misc/MessageDialog.h"
#include "Development/UHLDebugSystemSettings.h"
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"

static const FName UHLDebugSystemEditorTabName("UHLDebugSystemEditor");

#define LOCTEXT_NAMESPACE "FUHLDebugSystemEditorModule"

namespace UHLDebugSystemEditorFunctionLibrary
{
	static bool HasPlayWorld()
	{
		return GEditor->PlayWorld != nullptr;
	}

	static bool HasNoPlayWorld()
	{
		return !HasPlayWorld();
	}

	static bool IsOptionEnabled(const FGameplayTag DebugCategoryTag)
	{
		const UUHLDebugSystemSettings* Settings = GetDefault<UUHLDebugSystemSettings>();
		bool bEnabled = Settings->EnabledDebugCategories[DebugCategoryTag];
		return bEnabled;
	}

	static void ToggleDebugCategory_Clicked(const FGameplayTag DebugCategoryTag)
	{
		// TODO if HasPlayWorld toggle DebugSubsystemCategories instead or both settings and subsystem
		if (HasPlayWorld())
		{
			UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEditor->PlayWorld);
			if (GameInstance)
			{
				UUHLDebugSystemSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSystemSubsystem>();
				if (DebugSubsystem)
				{
					DebugSubsystem->ToggleDebugCategory(DebugCategoryTag);
				}
			}
		}
		
		UUHLDebugSystemSettings* Settings = GetMutableDefault<UUHLDebugSystemSettings>();
		bool bEnabled = Settings->EnabledDebugCategories[DebugCategoryTag];
		Settings->EnabledDebugCategories.Emplace(DebugCategoryTag, !bEnabled);	
		

		// if this controls editor UI, you may need to force a menu refresh:
		// FSlateApplication::Get().DismissAllMenus();
		// ExtendToolbar();  // or however you re-populate your menu
	}

	static TSharedRef<SWidget> GetDebugCategoriesStateDropdown()
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		MenuBuilder.BeginSection(NAME_None, FText::FromString("DebugCategories"));
		for (const FUHLDebugCategory& DebugCategory : GetDefault<UUHLDebugSystemSettings>()->DebugCategories)
		{
			const FText DisplayName = FText::FromString(DebugCategory.Tags.First().ToString());
			MenuBuilder.AddMenuEntry(
				DisplayName,
				FText::Format(LOCTEXT("CommonPathDescription", "{0}"), FText::FromString(DebugCategory.Name)),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::ToggleDebugCategory_Clicked, DebugCategory.Tags.First()),
					FCanExecuteAction::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::HasNoPlayWorld),
					FIsActionChecked::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::IsOptionEnabled, DebugCategory.Tags.First()),
					FIsActionButtonVisible::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::HasNoPlayWorld)
				),
				NAME_None,
				EUserInterfaceActionType::ToggleButton
			);
		}
		MenuBuilder.EndSection();

		return MenuBuilder.MakeWidget();
	}

	static bool CanShowDebugCategories()
	{
		return !GetDefault<UUHLDebugSystemSettings>()->EnabledDebugCategories.IsEmpty();
	}

	static void RegisterGameEditorMenus()
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = Menu->AddSection("PlayGameExtensions", TAttribute<FText>(),
		                                             FToolMenuInsert("Play", EToolMenuInsertType::Default));

		FToolMenuEntry CommonMapEntry = FToolMenuEntry::InitComboButton(
			"CommonMapOptions",
			FUIAction(
				FExecuteAction(),
				// FCanExecuteAction::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::HasNoPlayWorld),
				FCanExecuteAction(),
				FIsActionChecked(),
				FIsActionButtonVisible::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::CanShowDebugCategories)),
			FOnGetContent::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::GetDebugCategoriesStateDropdown),
			LOCTEXT("UHLDebugSystemEditor_Label", "Debug Categories"),
			LOCTEXT("UHLDebugSystemEditor_ToolTip", "Enabling/Disabling debug categories fast"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Tools")
		);
		CommonMapEntry.StyleNameOverride = "CalloutToolbar";
		Section.AddEntry(CommonMapEntry);
	}
}

void FUHLDebugSystemEditorModule::StartupModule()
{
	if (FSlateApplication::IsInitialized())
	{
		UToolMenus::RegisterStartupCallback(
			FSimpleMulticastDelegate::FDelegate::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::RegisterGameEditorMenus));
	}
}

void FUHLDebugSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLDebugSystemEditorModule, UHLDebugSystemEditor)
