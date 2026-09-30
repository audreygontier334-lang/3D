// Faux-semblants — interface du prologue, dessinée sans asset (AHUD + Canvas) :
// écran titre (menu), menu principal (commandes modifiables, son, options, aide, sauvegardes),
// carton d'ouverture, horloge, objectif, sous-titres, rappels, notifications, carnet d'indices,
// inventaire du sac en bandoulière, téléphone et écran de fin du prologue.
// Les textes de jeu viennent de GameData/ (UI_…, DLG_…, telephone) via les sous-systèmes.
// Codex pourra habiller cette interface (polices, cadres, sons) sans changer sa logique.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InputCoreTypes.h"
#include "FSHUD.generated.h"

class UFont;
class USoundMix;
class AFSPrologueDirector;
class AFSHeroCharacter;
class UFSMissionSubsystem;
class UFSPhoneSubsystem;

/** Pages d'interface qui prennent la main sur le clavier (voir AFSPlayerController). */
enum class EFSPage : uint8
{
	None, Title, Main, Controls, Sound, Options, Help, SaveSlots, LoadSlots, Inventory, Phone
};

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

	/** J : ouvre ou ferme le carnet. */
	void ToggleNotebook() { if (Page == EFSPage::None) { bNotebook = !bNotebook; } }

	bool IsOnTitleScreen() const { return Page == EFSPage::Title; }

	/** Joue plusieurs répliques à la suite (DLG_…), chacune le temps de la lire. */
	void PlayConversation(const TArray<FName>& LineIds);

	/** Éclair blanc bref et déclic : une photo vient d'être prise. */
	void Flash();

	/** Nouvel élément reçu sur le téléphone (message, mail, notification). */
	void NotifyPhone();

	// --- Menus, inventaire, téléphone -----------------------------------------------------------

	/** Vrai quand une page prend la main sur le clavier. */
	bool IsModal() const { return Page != EFSPage::None; }

	/** Touche reçue pendant une page modale (navigation, saisie, nouvelle commande). */
	void HandleKey(const FKey& Key, bool bRepeat);

	/** Ouvre une page ; si elle est déjà ouverte, la referme (touches P, I, O). */
	void OpenPage(EFSPage NewPage);

	/** Après un chargement : ferme les pages, efface sous-titres et carton, reprend le jeu. */
	void OnGameLoaded();

	/** Applique les volumes du menu Son. */
	void ApplyVolumes();

	/** Son d'interface (généré par Scripts/setup_prologue.py dans /Game/Audio). */
	void PlayUi(const TCHAR* SoundName);

private:
	struct FTimedText
	{
		FString Speaker;
		FString Text;
		float Until = 0.f;
	};

	struct FMenuRow
	{
		FString Label;
		FString Value;
	};

	UFUNCTION()
	void HandleAcquired(FName Id);

	float Now() const;
	float Ui() const;
	UFSMissionSubsystem* Mission() const;
	UFSPhoneSubsystem* Phone() const;
	AFSHeroCharacter* Hero() const;
	AFSPrologueDirector* Director();
	FString UIText(const TCHAR* Id, const TCHAR* Fallback) const;
	FString ClockText();

	// Jeu
	void DrawIntroCard();
	void DrawClockAndObjective();
	void DrawSubtitles();
	void DrawHintsAndToasts();
	void DrawNotebook();
	void DrawEndScreen();
	void DrawPrompt();
	void DrawAlleyMarker();
	void DrawFlash();
	void UpdateConversation();

	// Pages (FSHUDMenu.cpp)
	void StartNewGame();
	void Back();
	void GoTo(EFSPage NewPage);
	void UpdatePause();
	TArray<FMenuRow> Rows(EFSPage ForPage);
	void Activate(int32 Row);
	void Adjust(int32 Row, int32 Direction);
	void DrawPage();
	void DrawMenuList(const FString& Title, const TArray<FMenuRow>& List, const FString& Footer, bool bWithBackdrop);
	void DrawHelp();
	void DrawInventory();
	void DrawPhone();
	TArray<FName> EvidenceIds() const;
	bool IsActionKey(const TCHAR* ActionName, const FKey& Key) const;
	void PhoneKey(const FKey& Key, int32 Vertical, bool bConfirm, bool bBack);
	void PhoneCall(const FString& Number);

	// Dessin
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
	bool bNotebook = false;
	bool bBound = false;
	float FlashStart = -10.f;
	TArray<FName> Conversation;
	float ConversationNext = 0.f;

	EFSPage Page = EFSPage::Title;
	TArray<EFSPage> Stack;
	int32 Index = 0;
	int32 Scroll = 0;
	bool bCapturing = false;
	FString Status;

	enum class EPhoneScreen : uint8 { Home, Dial, Contacts, Log, Messages, Mails, Notifications, Detail };
	EPhoneScreen PhoneScreen = EPhoneScreen::Home;
	int32 PhoneIndex = 0;
	FString PhoneDial;
	FName PhoneDetail;
	FString PhoneLine;

	UPROPERTY() TObjectPtr<USoundMix> Mix;
	bool bMixPushed = false;
};
