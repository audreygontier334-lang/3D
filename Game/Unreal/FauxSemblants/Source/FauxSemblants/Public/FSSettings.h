// Faux-semblants — réglages de la joueuse (menu pause), gardés dans GameUserSettings.ini, section [FauxSemblants].
#pragma once

#include "CoreMinimal.h"

namespace FSSettings
{
	/** Multiplicateur de la souris, de 0,25 à 3 (1 par défaut). */
	FAUXSEMBLANTS_API float MouseSensitivity();
	FAUXSEMBLANTS_API void SetMouseSensitivity(float Value);

	FAUXSEMBLANTS_API bool InvertY();
	FAUXSEMBLANTS_API void SetInvertY(bool bValue);

	/** Taille des sous-titres : 1, 1,25 ou 1,5. */
	FAUXSEMBLANTS_API float SubtitleScale();
	FAUXSEMBLANTS_API void SetSubtitleScale(float Value);

	/** Option d'accessibilité OPT_TEMPS_ACTION_ETENDU (mission.json → prologue.window_accessibility). */
	FAUXSEMBLANTS_API bool ExtendedActionTime();
	FAUXSEMBLANTS_API void SetExtendedActionTime(bool bValue);
}
