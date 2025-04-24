// Pavel Penkov 2025 All Rights Reserved.

using UnrealBuildTool;

public class CommonMaps : ModuleRules
{
	public CommonMaps(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] 
			{
				"Core", 
				"DeveloperSettings",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"AssetTools",
				"ToolMenus",
				"ContentBrowser"
			}
		);
	}
}
