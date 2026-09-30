// Faux-semblants — metteur en scène du prologue (SC_P0_A → SC_P3), en temps réel.
// Chronologie : docs/narrative/DECOUPAGE_SCENES.md ; règles : GameData/missions/01/mission.json → prologue.
//  - l'alerte part quand la joueuse (ou Ariane) est à moins de alert_trigger_distance_m de l'entrée de la ruelle,
//    ou au plus tard à alert_latest ;
//  - fenêtre d'action de window_seconds, au plus window_max_actions actions (photo, courir, crier, envoyer Ariane) ;
//  - le départ est retenu tant que la joueuse n'est pas à l'entrée (hold_max_s au plus), sinon plan court CAM_DEPART_COURT.
// Les acteurs sont retrouvés par tag, posés par Scripts/setup_prologue.py.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSPrologueDirector.generated.h"

class UFSMissionSubsystem;
class UFSSaveGame;

/** Point de passage d'un figurant : secondes depuis 16:25:00, puis X est et Z nord en mètres (maquette glTF). */
struct FFSKey
{
	float T;
	float X;
	float Z;
};

UCLASS()
class FAUXSEMBLANTS_API AFSPrologueDirector : public AActor
{
	GENERATED_BODY()

public:
	AFSPrologueDirector();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Action de fenêtre demandée par la joueuse (ACT_PHOTO, ACT_CRIER, ACT_ENVOYER). ACT_COURIR est détectée. */
	void TryAction(FName ActionId);

	/** Coucou de Lila (SC_P0_B) : vrai pendant les secondes où la joueuse peut lui répondre d'un geste. */
	bool CanWave() const;

	/** Répondre au coucou : FLAG_COUCOU_RENDU, puis DLG_P_LILA_02 et la réaction d'Ariane. */
	bool TryWave();

	/** Sauvegarde et reprise de l'état du prologue (menu principal › Sauvegardes). */
	void WriteState(UFSSaveGame& Save) const;
	void ReadState(const UFSSaveGame& Save);

	/** Entrée de la ruelle (repère affiché par l'interface quand il faut y aller). */
	AActor* GetEntrance() const { return Entrance; }

	/** « Appeler le 17 » : fin du prologue, début du chapitre 1. */
	void CallPolice();

	/** Convertit un point de la maquette (mètres, X est, Z nord) en position Unreal (cm, X est, Y sud, Z haut). */
	static FVector FromBlockout(float X, float Z, float Height = 0.f) { return FVector(X * 100.f, -Z * 100.f, Height * 100.f); }

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	FString MissionFolder = TEXT("M01");

	/** Pour l'interface (AFSHUD) : 0 avant l'alerte, 1 fenêtre d'action, 2 retenue, 3 départ, 4 fourgon parti, 5 appel passé. */
	int32 GetPhaseIndex() const { return static_cast<int32>(Phase); }

	/** Secondes écoulées depuis 16:25:00. */
	float GetPrologueSeconds() const { return T; }

	/** Secondes restantes de la fenêtre d'action (0 hors fenêtre). */
	float GetWindowRemaining() const { return Phase == EPhase::Window ? FMath::Max(0.f, WindowSeconds - (T - AlertT)) : 0.f; }

	float GetWindowSeconds() const { return WindowSeconds; }

	int32 GetActionsLeft() const { return FMath::Max(0, MaxActions - ActionsTaken.Num()); }

	/** Accélère le prologue pour les tests (1 = temps réel). */
	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float TimeScale = 1.f;

private:
	enum class EPhase : uint8 { Before, Window, Holding, Departing, Gone, Chapter1 };

	void Say(const FString& Text, float Seconds = 6.f, int32 Key = 1);
	void SayLine(const TCHAR* LineId, float Seconds = 6.f);
	void MoveAlong(AActor* Actor, const TArray<FFSKey>& Keys, float T, float Height) const;
	void OpenWindow();
	void StartDeparture();
	void Grant(const TArray<FName>& Ids);
	// L'alerte accepte Ariane ; la retenue du départ attend uniquement la joueuse.
	float DistanceToEntrance(bool bIncludeDog = true) const;
	AActor* FindTagged(FName Tag) const;

	UPROPERTY() TObjectPtr<AActor> Lila;
	UPROPERTY() TObjectPtr<AActor> K2;
	UPROPERTY() TObjectPtr<AActor> Van;
	UPROPERTY() TObjectPtr<AActor> Entrance;
	UPROPERTY() TObjectPtr<AActor> Keyring;
	UPROPERTY() TObjectPtr<AActor> Bracelet;
	UPROPERTY() TObjectPtr<AActor> DepartCourtCamera;
	UPROPERTY() TObjectPtr<UFSMissionSubsystem> Mission;

	TArray<FFSKey> LilaPath;
	TArray<FFSKey> K2Path;

	EPhase Phase = EPhase::Before;
	float T = 0.f;             // secondes depuis 16:25:00
	float AlertT = -1.f;
	float DepartT = -1.f;
	float WindowSeconds = 10.f;
	int32 MaxActions = 2;
	float AlertLatest = 300.f; // 16:30:00
	float AlertDistanceCm = 500.f;
	float HoldMaxS = 30.f;
	float HoldDistanceCm = 500.f;
	float DescentS = 12.f;
	float ShotS = 4.f;
	TArray<FName> ActionsTaken;
	FVector VanStart = FVector::ZeroVector;
	bool bFallbackShot = false;
	int32 LastCue = -1;
	bool bWaved = false;
	bool bDogReacted = false;
	int32 LastPhoneEvent = -1;
	void PhoneEvent(const TCHAR* EventId);
	void ApplyPhaseVisibility();
};
