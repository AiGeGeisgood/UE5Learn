// Some copyright should be here...

using UnrealBuildTool;

public class XGSampleWSM : ModuleRules
{
	public XGSampleWSM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
             "$(ModuleDir)/Public/Subsystem",
             "$(ModuleDir)/Public/Type",
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "XGSampleWSM/Public/Connection",
                "XGSampleWSM/Public/Subsystem",
                "XGSampleWSM/Public/Type",
                "XGSampleWSM/Public/Log",
                "XGSampleWSM/Public/Util",
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "WebSocketNetworking",
                "WebSockets",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Json",
                "JsonUtilities"
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
