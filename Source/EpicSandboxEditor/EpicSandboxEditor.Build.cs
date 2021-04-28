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
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"UnrealEd",
			"AssetTools"
		}
		);
		
		// Runtime Module is needed as dependency
		PrivateDependencyModuleNames.AddRange(new [] { "EpicSandbox" });
	}
}