#include "FSPlayerController.h"
#include "FSHUD.h"

bool AFSPlayerController::InputKey(const FInputKeyParams& Params)
{
	if (AFSHUD* Hud = Cast<AFSHUD>(GetHUD()))
	{
		if (Hud->IsModal())
		{
			if (Params.Event == IE_Pressed || Params.Event == IE_Repeat)
			{
				// Les mouvements de souris et de sticks (axes) continuent de passer : on peut regarder autour de soi.
				if (!Params.Key.IsAxis1D() && !Params.Key.IsAxis2D())
				{
					Hud->HandleKey(Params.Key, Params.Event == IE_Repeat);
					return true;
				}
			}
			else if (Params.Event == IE_Released && !Params.Key.IsAxis1D() && !Params.Key.IsAxis2D())
			{
				// Relâchements transmis pour ne pas laisser une touche « enfoncée » (course, déplacement).
				return Super::InputKey(Params);
			}
		}
	}
	return Super::InputKey(Params);
}
