// Copyright (c) Andrew Bell

using UnrealBuildTool;

public class EpicSandboxEditor : ModuleRules
{
	public EpicSandboxEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new []
			{
				"Engine"
			}
			);
		// These are Engine modules
		// Since almost all the EpicSandbox Editor module is in the private, add them here
		PrivateDependencyModuleNames.AddRange(
			new []
		{
			"Core", 
			"CoreUObject",
			"UnrealEd",
			"SlateCore",
			"Slate", 
			"EditorStyle",
			"AssetDefinition",
			"EditorFramework",
			"PlacementMode"
		}
		);
		
		// Runtime Module is needed as dependency + MyCustomAssetEditor
		PrivateDependencyModuleNames.AddRange(new [] { "EpicSandbox", "MyCustomAssetEditor" });
	}
}
