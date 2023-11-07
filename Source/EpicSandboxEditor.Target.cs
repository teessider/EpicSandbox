// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class EpicSandboxEditorTarget : TargetRules
{
	public EpicSandboxEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;  // needed to avoid backwards compat mode
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange( new[] { "EpicSandbox", "EpicSandboxEditor", "MyCustomAssetEditor" } );
	}
}
