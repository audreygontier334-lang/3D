using UnrealBuildTool;

public class FauxSemblants : ModuleRules
{
	public FauxSemblants(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json" });
	}
}
