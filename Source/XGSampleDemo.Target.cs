// Copyright 2024 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class XGSampleDemoTarget : TargetRules
{
	public XGSampleDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("XGSampleDemo");
	}
}
