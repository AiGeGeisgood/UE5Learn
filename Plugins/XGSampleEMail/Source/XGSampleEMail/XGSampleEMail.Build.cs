// Some copyright should be here...

using UnrealBuildTool;

public class XGSampleEMail : ModuleRules
{
	public XGSampleEMail(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
             "$(ModuleDir)/Public/Type",
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "XGSampleEmail/Public/AsyncAction",
                "XGSampleEmail/Public/Log",
                "XGSampleEmail/Public/Subsystem",
                "XGSampleEmail/Public/Thread",
                "XGSampleEmail/Public/Type"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
                "Sockets",
                "Networking"


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
