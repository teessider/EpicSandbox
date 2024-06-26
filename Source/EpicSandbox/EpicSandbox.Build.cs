// Copyright (c) Andrew Bell

using UnrealBuildTool;

public class EpicSandbox : ModuleRules
{
	public EpicSandbox(ReadOnlyTargetRules Target) : base (Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new [] { "Core", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new [] { "CoreUObject", "RenderCore" });
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
