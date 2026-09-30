#include "FSGameMode.h"
#include "FSHeroCharacter.h"

AFSGameMode::AFSGameMode()
{
	DefaultPawnClass = AFSHeroCharacter::StaticClass();
}
