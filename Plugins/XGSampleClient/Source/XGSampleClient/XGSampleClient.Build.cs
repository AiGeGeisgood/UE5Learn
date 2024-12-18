// Some copyright should be here...

using UnrealBuildTool;

public class XGSampleClient : ModuleRules
{
	public XGSampleClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
             "$(ModuleDir)/Public/Type",
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
               "XGSampleClient/Public/Config",
               "XGSampleClient/Public/Log",
               "XGSampleClient/Public/Subsystem",
               "XGSampleClient/Public/Type",
               "XGSampleClient/Public/Util",
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "HTTP",
                "DeveloperSettings"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "JsonUtilities",
                "Json",
                "Projects"

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
