// Pavel Penkov 2025 All Rights Reserved.

#include "CommonMaps.h"
#include "AssetSelection.h"
#include "CommonMapCategoryCustomization.h"
#include "CommonMapsDeveloperSettings.h"
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "FCommonMapsModule"

namespace CommonMapsFunctionLibrary
{
	static bool HasPlayWorld()
	{
		return GEditor->PlayWorld != nullptr;
	}

	static bool HasNoPlayWorld()
	{
		return !HasPlayWorld();
	}

	static void OpenCommonMap_Clicked(const FString MapPath)
	{
		constexpr bool bPromptUserToSave = true;
		constexpr bool bSaveMapPackages = true;
		constexpr bool bSaveContentPackages = true;
		constexpr bool bFastSave = false;
		constexpr bool bNotifyNoPackagesSaved = false;
		constexpr bool bCanBeDeclined = true;
		if (FEditorFileUtils::SaveDirtyPackages(bPromptUserToSave, bSaveMapPackages, bSaveContentPackages, bFastSave,
		                                        bNotifyNoPackagesSaved, bCanBeDeclined))
		{
			if (ensure(MapPath.Len()))
			{
				GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(MapPath);
			}
		}
	}

	static TSharedRef<SWidget> GetCommonMapsDropdown()
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		for (const FCommonMapCategory& MapCategory : GetDefault<UCommonMapsDeveloperSettings>()->MapsCategories)
		{
			TAttribute<FText> SectionText;
			SectionText.Set(FText::FromName(MapCategory.CategoryName));
			MenuBuilder.BeginSection(NAME_None, SectionText);

			for (const FCommonMap& Map : MapCategory.Maps)
			{
				for (const FSoftObjectPath& MapURL : Map.MapURL)
				{
					if (!MapURL.IsValid())
					{
						continue;
					}
	
					const FText DisplayName = FText::FromString(MapURL.GetAssetName());
					MenuBuilder.AddMenuEntry(
						DisplayName,
						FText::Format(LOCTEXT("CommonPathDescription", "{0}"), FText::FromString(MapURL.ToString())),
						FSlateIcon(),
						FUIAction(
							FExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::OpenCommonMap_Clicked, MapURL.ToString()),
							FCanExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld),
							FIsActionChecked(),
							FIsActionButtonVisible::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld)
						)
					);
				}
			}

			if (MapCategory.bSearchMapsInFolder)
			{
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				TArray<FAssetData> MapAssets;
	
				FString Relative = MapCategory.SearchFolder.Path.TrimStartAndEnd();
				FString AssetRegistryPath = FString::Printf(TEXT("/Game/%s"), *Relative);
				FPaths::NormalizeDirectoryName(AssetRegistryPath);
				
				AssetRegistryModule.Get().GetAssetsByPath(FName(AssetRegistryPath), MapAssets, /*bRecursive=*/ true);
	
				if (MapAssets.Num() > 0)
				{
					for (const FAssetData& AssetData : MapAssets)
					{
						// Only include World assets (unreal maps have AssetClass "World")
						if (AssetData.AssetClassPath.GetAssetName() == FName("World"))
						{
							const FString AssetPathString = AssetData.GetSoftObjectPath().ToString();
							const FText DisplayName = FText::FromString(AssetData.AssetName.ToString());
	
							MenuBuilder.AddMenuEntry(
								DisplayName,
								FText::FromString(AssetPathString),
								FSlateIcon(),
								FUIAction(
									FExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::OpenCommonMap_Clicked, AssetPathString),
									FCanExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld),
									FIsActionChecked(),
									FIsActionButtonVisible::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld)
								)
							);
						}
					}
				}
			}
		}

		return MenuBuilder.MakeWidget();
	}

	static bool CanShowCommonMaps()
	{
		return HasNoPlayWorld() && !GetDefault<UCommonMapsDeveloperSettings>()->MapsCategories.IsEmpty();
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
				FCanExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld),
				FIsActionChecked(),
				FIsActionButtonVisible::CreateStatic(&CommonMapsFunctionLibrary::CanShowCommonMaps)),
			FOnGetContent::CreateStatic(&CommonMapsFunctionLibrary::GetCommonMapsDropdown),
			LOCTEXT("CommonMaps_Label", "Common Maps"),
			LOCTEXT("CommonMaps_ToolTip", "Some commonly desired maps while using the editor"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level")
		);
		CommonMapEntry.StyleNameOverride = "CalloutToolbar";
		Section.AddEntry(CommonMapEntry);
	}
}

void FCommonMapsModule::StartupModule()
{
	ExtendContextMenu();

	if (!IsRunningGame())
	{
		if (FSlateApplication::IsInitialized())
		{
			 UToolMenus::RegisterStartupCallback(
				FSimpleMulticastDelegate::FDelegate::CreateStatic(&CommonMapsFunctionLibrary::RegisterGameEditorMenus));
		}
	}

	// customization of "SearchFolder" to make "meta=(RelativePath)" work
	// FPropertyEditorModule& PropEd = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// PropEd.RegisterCustomPropertyTypeLayout(
	// 	"CommonMapCategory",
	// 	FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCommonMapCategoryCustomization::MakeInstance)
	// );
}

void FCommonMapsModule::ExtendContextMenu()
{
	UToolMenu* WorldAssetMenu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.World");
	if (!WorldAssetMenu) return;

	FToolMenuSection& Section = WorldAssetMenu->AddSection("AssetContextCommonMaps",
	                                                       LOCTEXT("CommonMapsOptionsMenuHeading", "Common Maps"));
	
	Section.AddSubMenu(FName("Common Maps"),
		LOCTEXT("AddToCommonMapsFromMenu", "Add to Common Maps"),
		LOCTEXT("AddToCommonMapsFromMenuTooltip", "Add this map into Common Maps list."),
		FNewMenuDelegate::CreateRaw(this, &FCommonMapsModule::CreateCategorySelectionSubmenu),
		FToolUIActionChoice(),
		EUserInterfaceActionType::None,
		false,
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Star"));
}

void FCommonMapsModule::AddToCommonMapsFromMenu(FName CategoryName)
{
	TArray<FAssetData> SelectedAssets;
	AssetSelectionUtils::GetSelectedAssets(SelectedAssets);
	if (auto* Settings = GetMutableDefault<UCommonMapsDeveloperSettings>())
	{
		for (const auto& AssetData : SelectedAssets)
		{
			if (FCommonMapCategory* CurrentCategory = Settings->MapsCategories.FindByKey(CategoryName))
			{
				CurrentCategory->Maps.Add(FCommonMap({ AssetData.GetSoftObjectPath() }));
			}
			else
			{
				Settings->MapsCategories.Add(FCommonMapCategory(CategoryName, { AssetData.GetSoftObjectPath() }));
			}
		}
		
		Settings->SaveConfig(CPF_Config);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("All maps added to Common Maps!"));
	}
}

void FCommonMapsModule::CreateCategorySelectionSubmenu(FMenuBuilder& MenuBuilder)
{
	const UCommonMapsDeveloperSettings* DevSettings = GetDefault<UCommonMapsDeveloperSettings>();
	if (!DevSettings) return;

	for (const FCommonMapCategory& Category : DevSettings->MapsCategories)
	{
		MenuBuilder.AddMenuEntry(
			FText::Format(LOCTEXT("CategoryLabel", "{0}"), FText::FromName(Category.CategoryName)),
			FText::Format(LOCTEXT("CategoryTooltip", "Add this map to \"{0}\" category."), FText::FromName(Category.CategoryName)),
			FSlateIcon(), FUIAction(FExecuteAction::CreateRaw(this, &FCommonMapsModule::AddToCommonMapsFromMenu, Category.CategoryName)));
	}
}


void FCommonMapsModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	// FPropertyEditorModule& PropEd = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// PropEd.UnregisterCustomPropertyTypeLayout("CommonMapCategory");
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommonMapsModule, CommonMaps)
