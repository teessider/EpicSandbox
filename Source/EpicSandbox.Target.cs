// Copyright (c) Andrew Bell

using UnrealBuildTool;
using System.Collections.Generic;

public class EpicSandboxTarget : TargetRules
{
	public EpicSandboxTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "EpicSandbox" } );
	}
}
