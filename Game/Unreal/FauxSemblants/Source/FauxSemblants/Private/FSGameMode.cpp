#include "FSGameMode.h"
#include "FSHeroCharacter.h"
#include "FSHUD.h"

AFSGameMode::AFSGameMode()
{
	DefaultPawnClass = AFSHeroCharacter::StaticClass();
	HUDClass = AFSHUD::StaticClass();
}
