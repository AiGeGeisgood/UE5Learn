// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XGSampleXFLink : ModuleRules
{
	public XGSampleXFLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
             "$(ModuleDir)/Public/AsyncAction",
             "$(ModuleDir)/Public/Subsystem",
             "$(ModuleDir)/Public/Thread",
             "$(ModuleDir)/Public/Type",
             "$(ModuleDir)/Public/Type/SilentBiopsy",
             "$(ModuleDir)/Public/Type/STT",
             "$(ModuleDir)/Public/Type/TTS",
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
             "XGSampleXFLink/Public/AsyncAction",
             "XGSampleXFLink/Public/Log",
             "XGSampleXFLink/Public/Subsystem",
             "XGSampleXFLink/Public/Thread",
             "XGSampleXFLink/Public/Type",
             "XGSampleXFLink/Public/Type/SilentBiopsy",
             "XGSampleXFLink/Public/Type/STT",
             "XGSampleXFLink/Public/Type/TTS",
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "HTTP",
                "XGSampleXFLinkLibrary",
				"Projects"

			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "CoreUObject",
                "Engine",
                "Json",
                "JsonUtilities",
                "AudioMixer",
                "AudioCapture",
                "WebSockets",

            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
			);

        //Need to make sure Android has Launch module.
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Launch"
                }
            );
        }
    }
}
