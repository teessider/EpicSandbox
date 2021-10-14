// Fill out your copyright notice in the Description page of Project Settings.

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
		
		// Since almost all the EpicSandbox Editor module is in the private, add them here
		PrivateDependencyModuleNames.AddRange(
			new []
		{
			"Core", 
			"CoreUObject",
			"UnrealEd",
			"AssetTools",
			"SlateCore",
			"Slate", 
			"EditorStyle"
		}
		);
		
		// Runtime Module is needed as dependency
		PrivateDependencyModuleNames.AddRange(new [] { "EpicSandbox" });
	}
}
