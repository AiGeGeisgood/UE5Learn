// Copyright 2024 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class XGSampleDemoEditorTarget : TargetRules
{
	public XGSampleDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("XGSampleDemo");
	}
}
