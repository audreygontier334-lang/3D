// Faux-semblants — état d'une mission : horloge de jeu, IDs acquis, déductions, textes.
// Lit les JSON de GameData/ copiés dans Content/Data/<mission>/ par Scripts/setup_prologue.py.
// Logique identique à Tools/validate_mission.py (derive, requirements_met) : les JSON restent la source.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dom/JsonObject.h"
#include "FSMissionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFSOnAcquired, FName, Id);

UCLASS()
class FAUXSEMBLANTS_API UFSMissionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Charge Content/Data/<MissionFolder>/ (mission.json, clues.json, deductions.json, dialogues.json). */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	bool LoadMission(const FString& MissionFolder);

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Grant(FName Id);

	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	bool Has(FName Id) const { return Acquired.Contains(Id); }

	/** Heure de jeu en minutes depuis minuit (ex. 16 h 31 = 991). */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	int32 GetClockMinutes() const { return ClockMinutes; }

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void SetClockMinutes(int32 Minutes) { ClockMinutes = Minutes; }

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void AdvanceClock(int32 Minutes) { ClockMinutes += Minutes; }

	/** Texte d'une réplique (DLG_…), jeton {HEROINE} remplacé. */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	FString GetLineText(FName LineId) const;

	/** Nom affiché de la personne qui dit la réplique ; vide pour la narration (carnet). */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	FString GetLineSpeaker(FName LineId) const;

	/** Texte d'interface (UI_…) de dialogues.json, jeton {HEROINE} remplacé ; Fallback si absent. */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	FString GetUIText(FName UIId, const FString& Fallback = TEXT("")) const;

	/** Indices (CLU_…) acquis, dans l'ordre où ils ont été notés. */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	TArray<FName> GetAcquiredClues() const;

	/** Nom court d'un indice (« Photo nette du fourgon »). */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	FString GetClueName(FName ClueId) const;

	/** Nature d'un indice : objet, document, observation, temoignage, resultat. */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	FString GetClueKind(FName ClueId) const;

	/** Remplace l'ensemble des IDs acquis (chargement d'une sauvegarde), dans l'ordre donné. */
	void RestoreAcquired(const TArray<FName>& Ids);

	/** IDs acquis dans l'ordre (sauvegarde). */
	const TArray<FName>& GetAcquiredOrder() const { return AcquiredOrder; }

	/** Fait d'un indice (CLU_…). */
	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	FString GetClueFact(FName ClueId) const;

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	bool SaveProgress(const FString& SlotName = TEXT("FauxSemblants"));

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	bool LoadProgress(const FString& SlotName = TEXT("FauxSemblants"));

	/** Objet JSON « prologue » de mission.json (actions, fenêtre, règle de départ). */
	TSharedPtr<FJsonObject> GetPrologue() const;

	/** Convertit « 16:31 » ou « 16:30:00 » en secondes depuis minuit. */
	static int32 ParseClockSeconds(const FString& Text);

	UPROPERTY(BlueprintAssignable, Category = "FauxSemblants")
	FFSOnAcquired OnAcquired;

	/** Nom affiché de l'héroïne tant qu'Audrey ne l'a pas choisi. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FauxSemblants")
	FString HeroineName = TEXT("Audrey");

	static const int32 DataVersion = 1;

private:
	void DeriveDeductions();
	static TSharedPtr<FJsonObject> ReadJson(const FString& Path);

	TSet<FName> Acquired;
	TArray<FName> AcquiredOrder;
	int32 ClockMinutes = 0;
	FString MissionFolder;

	TSharedPtr<FJsonObject> Mission;
	TMap<FName, FString> ClueFacts;
	TMap<FName, FString> ClueNames;
	TMap<FName, FString> ClueKinds;
	TMap<FName, FString> Lines;
	TMap<FName, FString> LineSpeakers;
	TMap<FName, FString> SpeakerNames;
	TMap<FName, FString> UITexts;
	/** Déduction → ensembles d'IDs dont un seul, complet, suffit (any_of). */
	TMap<FName, TArray<TArray<FName>>> Deductions;
};
