// Copyright (c) Andrew Bell

using UnrealBuildTool;

public class MyCustomAssetEditor : ModuleRules
{
	public MyCustomAssetEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new []
			{
				"Engine"
			}
			);
		
		// Since almost all the MyCustomAssetEditor module is in the private, add them here
		PrivateDependencyModuleNames.AddRange(
			new []
		{
			"Core", 
			"CoreUObject",
			"UnrealEd",
			"EditorStyle",
			"SlateCore",
			"Slate",
			"AdvancedPreviewScene",
			"InputCore",
			"ToolMenus",
			"CommonMenuExtensions", // Needed for FShowFlagFilter & FShowFlagMenuCommands
			"RHI"
		}
		);
		
		// Runtime Module is needed as dependency
		PrivateDependencyModuleNames.AddRange(new [] { "EpicSandbox" });
	}
}