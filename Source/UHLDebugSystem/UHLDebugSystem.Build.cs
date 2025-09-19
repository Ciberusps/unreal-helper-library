// Pavel Penkov 2025 All Rights Reserved.

using UnrealBuildTool;

// DebugSystem based on GameplayTags that completely separate from other UHL modules
// should be completely independent of UHL and probably will be other plugin
public class UHLDebugSystem : ModuleRules
{
	public UHLDebugSystem(ReadOnlyTargetRules Target) : base(Target)
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
				"CoreUObject",
				"Engine",
				
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
				"Slate",
				"SlateCore",
				"UMG",
				"AnimGraphRuntime",
				"DeveloperSettings", 
				
				// Should not use any other UHL modules
				// ... add private dependencies that you statically link with here ...
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
