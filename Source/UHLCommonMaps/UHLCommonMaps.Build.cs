// Pavel Penkov 2025 All Rights Reserved.

using UnrealBuildTool;

public class UHLCommonMaps : ModuleRules
{
	public UHLCommonMaps(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] 
			{
				"Core", 
				"DeveloperSettings", 
				"PropertyEditor",
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
				"DesktopWidgets",
			}
		);
	}
}
