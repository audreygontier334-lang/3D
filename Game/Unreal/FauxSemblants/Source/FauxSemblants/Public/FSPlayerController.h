// Faux-semblants — contrôleur de la joueuse : quand un menu, l'inventaire ou le téléphone est ouvert,
// les touches vont d'abord à l'interface (navigation, saisie d'un numéro, nouvelle touche à assigner).
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FSPlayerController.generated.h"

UCLASS()
class FAUXSEMBLANTS_API AFSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual bool InputKey(const FInputKeyParams& Params) override;
};
