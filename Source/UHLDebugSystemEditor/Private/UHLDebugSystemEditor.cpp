// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugSystemEditor.h"

#include "FileHelpers.h"
#include "Development/UHLDebugSystemSettings.h"
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"

static const FName UHLDebugSystemEditorTabName("UHLDebugSystemEditor");

#define LOCTEXT_NAMESPACE "FUHLDebugSystemEditorModule"

namespace UHLDebugSystemEditorFunctionLibrary
{
	static bool HasPlayWorld() { return GEditor->PlayWorld != nullptr; }

	static bool HasNoPlayWorld() { return !HasPlayWorld(); }

	static bool IsOptionEnabled(const FGameplayTag DebugCategoryTag)
	{
		if (HasPlayWorld())
		{
			UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEditor->PlayWorld);
			if (!GameInstance) return false;

			UUHLDebugSystemSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSystemSubsystem>();
			if (!DebugSubsystem) return false;
			
			return DebugSubsystem->IsCategoryEnabled(DebugCategoryTag);
		}
		else
		{
			const UUHLDebugSystemSettings* Settings = GetDefault<UUHLDebugSystemSettings>();
			bool bEnabled = Settings->EnabledDebugCategories[DebugCategoryTag];
			return bEnabled;	
		}
	}

	static void ToggleDebugCategory_Clicked(const FGameplayTag DebugCategoryTag)
	{
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
    	Settings->OnEnabledDebugCategoryEntryChanged(DebugCategoryTag, !bEnabled);
		
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
				FText::Format(LOCTEXT("DebugCategoryDescription", "{0}"), FText::FromString(DebugCategory.Name)),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::ToggleDebugCategory_Clicked, DebugCategory.Tags.First()),
					FCanExecuteAction(),
					// FCanExecuteAction::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::HasNoPlayWorld),
					FIsActionChecked::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::IsOptionEnabled, DebugCategory.Tags.First()),
					FIsActionButtonVisible()
					// FIsActionButtonVisible::CreateStatic(&UHLDebugSystemEditorFunctionLibrary::HasNoPlayWorld)
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

		FToolMenuEntry ToolMenuEntry = FToolMenuEntry::InitComboButton(
			"DebugCategoriesState",
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
		ToolMenuEntry.StyleNameOverride = "CalloutToolbar";
		Section.AddEntry(ToolMenuEntry);
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
	UToolMenus::UnRegisterStartupCallback(this);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUHLDebugSystemEditorModule, UHLDebugSystemEditor)
