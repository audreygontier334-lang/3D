using UnrealBuildTool;

public class FauxSemblantsEditorTarget : TargetRules
{
	public FauxSemblantsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("FauxSemblants");
	}
}
