using UnrealBuildTool;

public class FauxSemblants : ModuleRules
{
	public FauxSemblants(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// UE 5.8 n'ajoute plus automatiquement la racine du module aux chemins d'inclusion :
		// Private/*.cpp doivent pouvoir inclure "FauxSemblants.h".
		PrivateIncludePaths.Add(ModuleDirectory);
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json" });
	}
}
