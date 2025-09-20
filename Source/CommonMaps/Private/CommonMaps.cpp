// Pavel Penkov 2025 All Rights Reserved.

#include "CommonMaps.h"
#include "AssetSelection.h"
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

	// Recursive helper to either add entries or deeper sub-menus.
    static void PopulateRecursive(
        FMenuBuilder& Builder,
        TArray<FAssetData> Assets,       // captured by value
        const FString RootPath,          // e.g. "/Game/GameName/Characters"
        int32 MaxDepth,                  // SubmenuDepth
        int32 CurrentDepth               // 0 on first call
    )
    {
        // Base case: we have descended MaxDepth levels, so just list maps here
        if (CurrentDepth >= MaxDepth)
        {
            for (const FAssetData& Asset : Assets)
            {
                const FString AssetPathString = Asset.GetSoftObjectPath().ToString();
                const FText   DisplayName      = FText::FromString(Asset.AssetName.ToString());

                Builder.AddMenuEntry(
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
            return;
        }

        // Otherwise, bucket by the folder name at this level
        TMap<FString, TArray<FAssetData>> Buckets;
        const FString Prefix = RootPath + TEXT("/");

        for (const FAssetData& Asset : Assets)
        {
            // Only World assets
            if (Asset.AssetClassPath.GetAssetName() != FName("World"))
                continue;

            // "/Game/Root/Folder1/Folder2" => "Folder1/Folder2"
            FString Rel = Asset.PackagePath.ToString();
            if (Rel.StartsWith(Prefix))
            {
                Rel = Rel.Mid(Prefix.Len());
            }
            else
            {
                Rel.Empty();
            }

            // Split into ["Folder1","Folder2",...]
            TArray<FString> Parts;
            Rel.ParseIntoArray(Parts, TEXT("/"), /*CullEmpty=*/ true);

            // Grab the segment at our current depth (0-based)
            FString Key = Parts.IsValidIndex(CurrentDepth) ? Parts[CurrentDepth] : TEXT("");
            Buckets.FindOrAdd(Key).Add(Asset);
        }

        // Create one submenu per bucket, capturing each subgroup by value
        for (auto& Pair : Buckets)
        {
            const FString& FolderName = Pair.Key.IsEmpty() ? TEXT("<root>") : Pair.Key;
            TArray<FAssetData> SubAssets = MoveTemp(Pair.Value);
            int32 NextDepth = CurrentDepth + 1;

            Builder.AddSubMenu(
                FText::FromString(FolderName),
                FText(),  // no extra tooltip
                FNewMenuDelegate::CreateLambda(
                    [SubAssets, RootPath, MaxDepth, NextDepth](FMenuBuilder& SubMenuBuilder)
                    {
                        // recurse with copies only
                        CommonMapsFunctionLibrary::PopulateRecursive(
                            SubMenuBuilder,
                            SubAssets,
                            RootPath,
                            MaxDepth,
                            NextDepth
                        );
                    }
                )
            );
        }
    }

	static TSharedRef<SWidget> GetCommonMapsDropdown()
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		for (const FCommonMapCategory& MapCategory : GetDefault<UCommonMapsDeveloperSettings>()->MapsCategories)
		{
			TAttribute<FText> SectionText;
			SectionText.Set(FText::FromName(MapCategory.Name));
			MenuBuilder.BeginSection(NAME_None, SectionText);

			for (const FSoftObjectPath& Map : MapCategory.Maps)
			{
				if (!Map.IsValid())
				{
					continue;
				}

				const FText DisplayName = FText::FromString(Map.GetAssetName());
				MenuBuilder.AddMenuEntry(
					DisplayName,
					FText::Format(LOCTEXT("CommonPathDescription", "{0}"), FText::FromString(Map.ToString())),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::OpenCommonMap_Clicked, Map.ToString()),
						FCanExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld),
						FIsActionChecked(),
						FIsActionButtonVisible::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld)
					)
				);
			}

			if (MapCategory.bAutoSearchMapsInFolder)
			{
				// 1) Normalize root (assume always "/Game/..."), strip trailing slash
				FString RootPath = MapCategory.SearchFolder.Path.TrimStartAndEnd();
				RootPath.RemoveFromEnd("/");
			
				// Gather all map assets under that path
				FAssetRegistryModule& AR = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				TArray<FAssetData> AllAssets;
				AR.Get().GetAssetsByPath(FName(*RootPath), AllAssets, /*bRecursive=*/ true);

				// Kick off the recursive submenu build at depth=0
				CommonMapsFunctionLibrary::PopulateRecursive(
					MenuBuilder,
					MoveTemp(AllAssets),
					RootPath,
					MapCategory.SubmenuDepth,
					/*CurrentDepth=*/ 0
				);
			}

			MenuBuilder.EndSection();
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
				CurrentCategory->Maps.Add({ AssetData.GetSoftObjectPath() });
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
			FText::Format(LOCTEXT("CategoryLabel", "{0}"), FText::FromName(Category.Name)),
			FText::Format(LOCTEXT("CategoryTooltip", "Add this map to \"{0}\" category."), FText::FromName(Category.Name)),
			FSlateIcon(), FUIAction(FExecuteAction::CreateRaw(this, &FCommonMapsModule::AddToCommonMapsFromMenu, Category.Name)));
	}
}


void FCommonMapsModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommonMapsModule, CommonMaps)
