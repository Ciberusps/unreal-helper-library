// Pavel Penkov 2025 All Rights Reserved.

using UnrealBuildTool;

// Module that helps debugging UnrealHelperLibrary and can depend on all other modules
// Don't mess with UHLDebugSystem - that completely independent system from UHL
// and probably should be other plugin 
public class UHLDebug : ModuleRules
{
	public UHLDebug(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"EnhancedInput",  
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				
				"Slate",
				"SlateCore",
				"UMG",
				"DeveloperSettings",

				"UnrealHelperLibrary",
				"UHLDebugSystem",
				"UHLGAS",
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
