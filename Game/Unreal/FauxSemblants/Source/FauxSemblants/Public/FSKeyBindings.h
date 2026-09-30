// Faux-semblants — commandes modifiables par la joueuse (menu principal › Modifier les commandes).
// Travaille sur les « Action Mappings » et « Axis Mappings » du projet (Config/DefaultInput.ini) ;
// les choix de la joueuse sont enregistrés dans son Input.ini (Saved/Config), la manette n'est pas modifiée.
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"

struct FFSBinding
{
	const TCHAR* Label;   // texte affiché
	const TCHAR* Name;    // ActionName ou AxisName
	float AxisScale;      // 0 = action ; sinon sens de l'axe (+1 / -1)
	TArray<FKey> Defaults;
};

namespace FSKeyBindings
{
	/** Liste des commandes réassignables, dans l'ordre du menu. */
	FAUXSEMBLANTS_API const TArray<FFSBinding>& All();

	/** Touches clavier et souris actuellement liées (« Z / W »). */
	FAUXSEMBLANTS_API FString KeysText(const FFSBinding& Binding);

	/** Remplace les touches clavier et souris de la commande par NewKey ; renvoie la commande qui l'utilisait déjà (échangée), ou une chaîne vide. */
	FAUXSEMBLANTS_API FString Rebind(const FFSBinding& Binding, const FKey& NewKey);

	/** Remet toutes les commandes du clavier comme à l'origine. */
	FAUXSEMBLANTS_API void ResetDefaults();

	/** Vrai pour une touche de clavier ou un bouton de souris (pas la manette, pas un axe). */
	FAUXSEMBLANTS_API bool IsAssignable(const FKey& Key);
}
