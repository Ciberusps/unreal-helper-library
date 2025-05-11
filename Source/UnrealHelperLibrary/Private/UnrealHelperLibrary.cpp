// Pavel Penkov 2025 All Rights Reserved.

#include "UnrealHelperLibrary.h"

#include "Misc/Paths.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FUnrealHelperLibraryModule"

DEFINE_LOG_CATEGORY(LogUnrealHelperLibrary);

void FUnrealHelperLibraryModule::StartupModule()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TagsManager.AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UnrealHelperLibrary/Config/Tags"));

	//   TArray<TSharedPtr<FGameplayTagNode>> OutTagArray;
	//   TArray<FString> OutPaths;
	//   TagsManager.GetTagSourceSearchPaths(OutPaths);
	//   TagsManager.GetAllTagsFromSource(FApp::GetProjectName(), OutTagArray);
	//   // GameplayTagsManager.FindTagsWithSource()
	//
	//   // TODO find MainModule name
	//   FString Test1 = FApp::GetName();
	//   FString Test2 = FApp::GetProjectName();
	//   FString Test3 = FApp::GetInstanceName();
	//   FString Test6 = FModuleManager::Get().GetGameBinariesDirectory();
	//
	//   // TArray<FModuleStatus> OutModuleStatuses;
	//   // FModuleManager::Get().QueryModules(OutModuleStatuses);
	//
	//   // FModuleStatus* MyGameModule = OutModuleStatuses.FindByPredicate([](const FModuleStatus& ModuleStatusStatus) {
	//   //     return ModuleStatusStatus.bIsGameModule && ModuleStatusStatus.bIsLoaded;
	//   // });
	//
	//   // Ask the module manager for a list of currently-loaded gameplay modules
	//   UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddLambda([this]()
	//   {
	//       UGameplayTagsManager& TagsManager2 = UGameplayTagsManager::Get();
	//
	//       // StaticImpl.RootTag = UGameplayTagsManager::Get().AddNativeGameplayTag(TagT::GetRootTagStr());
	// TArray<FString> GameModulesNames;
	// TArray<FModuleStatus> ModuleStatuses;
	// FModuleManager::Get().QueryModules(ModuleStatuses);
	//
	// for (FModuleStatus& ModuleStatus : ModuleStatuses)
	// {
	// 	// We only care about game modules that are currently loaded
	// 	if (ModuleStatus.bIsLoaded && ModuleStatus.bIsGameModule)
	// 	{
	// 		GameModulesNames.Add(MoveTemp(ModuleStatus.Name));
	// 	}
	// }
	//
	// for (const FString& GameModulesName : GameModulesNames)
	// {
	// 	// GameplayTagsManager.FindTagsWithSource(FString("Bogatyr"), NativeTags);
	// 	TArray<FGameplayTag> NativeTags;
	// 	TagsManager2.FindTagsWithSource(GameModulesName, NativeTags);
	//
	// 	for (FGameplayTag& NativeTag : NativeTags)
	// 	{
	// 		FName NativeTagNam = NativeTag.GetTagName();
	// 	    // FNativeGameplayTag* Test = new FNativeGameplayTag(FName("UnrealHelperLibrary"), FName("UnrealHelperLibrary"), NativeTagNam, "", ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	// 	    // TagsManager2.AddNativeGameplayTag(Test);
	// 		TagsManager2.AddNativeGameplayTag(NativeTagNam, "");
	// 	}
	// }
	//       FName TestTag = "Test123.Test322.Test123";
	//       TagsManager2.AddNativeGameplayTag(TestTag, "");
	// // UGameplayTagsManager::Get().DoneAddingNativeTags();
	//   });
	//
	//   TagsManager.ConstructGameplayTagTree();
	//   TagsManager.LoadGameplayTagTables();

	// for (const FGameplayTag& Tag : NativeTags)
	// {
	// 	if (IsTagOnlyAvailableFromExcludedSources(Manager, Tag, ExcludedPlugins))
	// 	{
	// 		FAssetIdentifier TagId = FAssetIdentifier(FGameplayTag::StaticStruct(), Tag.GetTagName());
	// 		AssetIdsInPlugin.Add(TagId);
	// 	}
	// }
	// }
}

void FUnrealHelperLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealHelperLibraryModule, UnrealHelperLibrary)
