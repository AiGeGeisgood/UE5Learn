// Copyright 2024 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGSampleDemo : ModuleRules
{
	public XGSampleDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "XGSampleDemo"
            }
        );




        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
            "GameplayTags",
            "DeveloperSettings",
            "ControlFlows",
            "Json",
            "JsonUtilities",
            "HTTP",
			"NetCore",
			"UMG"

        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
