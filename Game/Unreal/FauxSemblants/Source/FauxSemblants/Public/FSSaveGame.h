// Faux-semblants — sauvegarde locale (hors ligne) : heure de jeu, IDs acquis, version des données.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FSSaveGame.generated.h"

UCLASS()
class FAUXSEMBLANTS_API UFSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString MissionFolder;

	UPROPERTY()
	int32 ClockMinutes = 0;

	UPROPERTY()
	int32 DataVersion = 0;

	UPROPERTY()
	TArray<FName> Acquired;
};
