// Copyright (c) Andrew Bell

using UnrealBuildTool;
using System.Collections.Generic;

public class EpicSandboxEditorTarget : TargetRules
{
	public EpicSandboxEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;  // needed to avoid backwards compat mode
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange( new[] { "EpicSandbox", "EpicSandboxEditor", "MyCustomAssetEditor" } );
	}
}
