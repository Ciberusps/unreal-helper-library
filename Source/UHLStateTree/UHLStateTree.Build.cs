// Pavel Penkov 2025 All Rights Reserved.

using UnrealBuildTool;

public class UHLStateTree : ModuleRules
{
	public UHLStateTree(ReadOnlyTargetRules Target) : base(Target)
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

				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				
				"AIModule",

				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",

				"StateTreeModule",
				"GameplayStateTreeModule"
			}
			);


		// DynamicallyLoadedModuleNames.AddRange(
		// 	new string[]
		// 	{
		// 		"StateTreeModule",
		// 		"GameplayStateTreeModule"
		// 		// ... add any modules that your module loads dynamically here ...
		// 	}
		// 	);
	}
}
