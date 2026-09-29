using UnrealBuildTool;

public class FauxSemblantsTarget : TargetRules
{
	public FauxSemblantsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("FauxSemblants");
	}
}
