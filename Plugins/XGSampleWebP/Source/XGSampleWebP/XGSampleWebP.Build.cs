// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XGSampleWebP : ModuleRules
{
	public XGSampleWebP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {

             "$(ModuleDir)/Public/Core",
             "$(ModuleDir)/Public/Subsystem",
             "$(ModuleDir)/Public/Type",


            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

             "XGSampleWebP/Public/Core",
             "XGSampleWebP/Public/Log",
             "XGSampleWebP/Public/Subsystem",
             "XGSampleWebP/Public/Type",

            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"XGSampleWebPLibrary",
				"Projects",
				"Engine"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "RenderCore",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
