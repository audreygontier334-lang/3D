#include "FSGameMode.h"
#include "FSHeroCharacter.h"
#include "FSHUD.h"
#include "FSPlayerController.h"

AFSGameMode::AFSGameMode()
{
	DefaultPawnClass = AFSHeroCharacter::StaticClass();
	HUDClass = AFSHUD::StaticClass();
	PlayerControllerClass = AFSPlayerController::StaticClass();
}
