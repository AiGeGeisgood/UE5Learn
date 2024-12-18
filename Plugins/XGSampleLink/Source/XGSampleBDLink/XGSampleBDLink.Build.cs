// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XGSampleBDLink : ModuleRules
{
	public XGSampleBDLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
             "$(ModuleDir)/Public/AsyncAction",
             "$(ModuleDir)/Public/Type",
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
             "XGSampleBDLink/Public/AsyncAction",
             "XGSampleBDLink/Public/Log",
             "XGSampleBDLink/Public/Type",


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
