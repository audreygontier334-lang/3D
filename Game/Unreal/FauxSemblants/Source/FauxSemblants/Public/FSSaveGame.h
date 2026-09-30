// Faux-semblants — sauvegarde locale (hors ligne) : heure de jeu, IDs acquis, version des données,
// et l'état complet du prologue (directeur, héroïne, Ariane, inventaire, téléphone) pour reprendre à l'identique.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FSSaveGame.generated.h"

UCLASS()
class FAUXSEMBLANTS_API UFSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY() FString MissionFolder;
	UPROPERTY() int32 ClockMinutes = 0;
	UPROPERTY() int32 DataVersion = 0;
	UPROPERTY() TArray<FName> Acquired;

	// Résumé affiché dans le menu.
	UPROPERTY() FDateTime SavedAt;
	UPROPERTY() FString Summary;

	// Directeur du prologue.
	UPROPERTY() bool bHasPrologue = false;
	UPROPERTY() float PrologueSeconds = 0.f;
	UPROPERTY() uint8 Phase = 0;
	UPROPERTY() float AlertT = -1.f;
	UPROPERTY() float DepartT = -1.f;
	UPROPERTY() float WindowSeconds = 10.f;
	UPROPERTY() TArray<FName> ActionsTaken;
	UPROPERTY() int32 LastCue = -1;
	UPROPERTY() bool bWaved = false;

	// Héroïne et Ariane.
	UPROPERTY() FTransform HeroTransform;
	UPROPERTY() FRotator ControlRotation = FRotator::ZeroRotator;
	UPROPERTY() uint8 CameraMode = 0;
	UPROPERTY() FVector DogLocation = FVector::ZeroVector;
	UPROPERTY() int32 Candies = 12;
	UPROPERTY() bool bGloves = false;
	UPROPERTY() bool bTorch = false;
	UPROPERTY() bool bTalkedToDufau = false;
	UPROPERTY() TArray<FName> PickedUp;

	// Téléphone.
	UPROPERTY() TArray<FName> PhoneDelivered;
	UPROPERTY() TArray<FName> PhoneRead;
	UPROPERTY() TArray<FString> PhoneCalls;

	/** Emplacements proposés dans le menu : 1, 2, 3 et la sauvegarde rapide. */
	static FString SlotName(int32 Index);

	/** Enregistre la partie en cours dans l'emplacement. */
	static bool SaveSlot(const UObject* WorldContext, int32 Index);

	/** Recharge l'emplacement dans la partie en cours (même niveau). */
	static bool LoadSlot(const UObject* WorldContext, int32 Index);

	/** « Mardi 16:27 — enregistré le 01/10 à 01:40 », ou vide si l'emplacement est libre. */
	static FString Describe(int32 Index);
};
