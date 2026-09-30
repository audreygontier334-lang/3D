// Faux-semblants — interface du prologue, dessinée sans asset (AHUD + Canvas) :
// écran titre, carton d'ouverture, horloge, objectif, sous-titres, rappels de commandes,
// notifications, carnet d'indices (J) et écran de fin du prologue.
// Les textes viennent de GameData/dialogues/01-ouverture.json (UI_…, DLG_…) via UFSMissionSubsystem.
// Codex pourra habiller cette interface (polices, cadres, sons) sans changer sa logique.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FSHUD.generated.h"

class UFont;
class AFSPrologueDirector;
class UFSMissionSubsystem;

UCLASS()
class FAUXSEMBLANTS_API AFSHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	/** Le HUD du joueur local, s'il existe. */
	static AFSHUD* Get(const UObject* WorldContext);

	/** Réplique sous-titrée ; Speaker vide = narration (texte du carnet, non voisé). */
	void ShowLine(const FString& Speaker, const FString& Text, float Seconds = 6.f);

	/** Rappel de commandes au-dessus des sous-titres ; Key remplace le rappel précédent de même clé. */
	void ShowHint(int32 Key, const FString& Text, float Seconds);

	/** Petite notification en haut à droite (vue changée, sauvegarde, indice noté…). */
	void ShowToast(const FString& Text, float Seconds = 2.5f);

	/** Entrée / Start : quitte l'écran titre et lance le prologue. */
	void PressStart();

	/** J : ouvre ou ferme le carnet. */
	void ToggleNotebook() { bNotebook = !bNotebook; }

	bool IsOnTitleScreen() const { return bTitle; }

	/** Joue plusieurs répliques à la suite (DLG_…), chacune le temps de la lire. */
	void PlayConversation(const TArray<FName>& LineIds);

	/** Éclair blanc bref : une photo vient d'être prise. */
	void Flash() { FlashStart = Now(); }

	/** P, Échap ou Start : ouvre ou ferme le menu pause (le jeu est suspendu pendant le menu). */
	void TogglePauseMenu();
	bool IsMenuOpen() const { return bMenu; }
	void MenuMove(int32 Direction);
	void MenuAdjust(int32 Direction);
	void MenuConfirm();

private:
	struct FTimedText
	{
		FString Speaker;
		FString Text;
		float Until = 0.f;
	};

	UFUNCTION()
	void HandleAcquired(FName Id);

	float Now() const;
	float Ui() const;
	UFSMissionSubsystem* Mission() const;
	AFSPrologueDirector* Director();
	FString UIText(const TCHAR* Id, const TCHAR* Fallback) const;

	void DrawTitle();
	void DrawIntroCard();
	void DrawClockAndObjective();
	void DrawSubtitles();
	void DrawHintsAndToasts();
	void DrawNotebook();
	void DrawEndScreen();
	void DrawPrompt();
	void DrawAlleyMarker();
	void DrawFlash();
	void DrawPauseMenu();
	void UpdateConversation();

	void Box(float X, float Y, float W, float H, const FLinearColor& Color);
	void Text(const FString& S, float X, float Y, UFont* Font, float Scale, const FLinearColor& Color, bool bCentreX = false);
	FVector2D Measure(const FString& S, UFont* Font, float Scale);
	TArray<FString> Wrap(const FString& S, UFont* Font, float Scale, float MaxWidth);

	TWeakObjectPtr<AFSPrologueDirector> CachedDirector;
	FTimedText Subtitle;
	TMap<int32, FTimedText> Hints;
	TArray<FTimedText> Toasts;
	FString IntroText;
	float IntroStart = -1.f;
	bool bTitle = true;
	bool bNotebook = false;
	bool bBound = false;
	bool bMenu = false;
	int32 MenuIndex = 0;
	float FlashStart = -10.f;
	TArray<FName> Conversation;
	float ConversationNext = 0.f;
};
