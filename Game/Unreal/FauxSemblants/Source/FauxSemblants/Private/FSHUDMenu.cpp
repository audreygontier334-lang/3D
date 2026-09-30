// Faux-semblants — pages de l'interface : écran titre, menu principal, commandes, son, options, aide,
// sauvegardes, inventaire du sac en bandoulière et téléphone. Suite de FSHUD.cpp (même classe AFSHUD).
#include "FSHUD.h"
#include "FauxSemblants.h"
#include "FSDogCharacter.h"
#include "FSHeroCharacter.h"
#include "FSKeyBindings.h"
#include "FSMissionSubsystem.h"
#include "FSPhoneSubsystem.h"
#include "FSPrologueDirector.h"
#include "FSSaveGame.h"
#include "FSSettings.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Engine/World.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

namespace
{
	const FLinearColor Cream(1.f, 0.93f, 0.80f, 1.f);
	const FLinearColor Amber(1.f, 0.72f, 0.28f, 1.f);
	const FLinearColor Soft(0.80f, 0.80f, 0.80f, 1.f);
	const FLinearColor Dim(0.55f, 0.55f, 0.55f, 1.f);
	const FLinearColor Panel(0.06f, 0.05f, 0.04f, 0.94f);

	bool IsUp(const FKey& K) { return K == EKeys::Up || K == EKeys::Gamepad_DPad_Up || K == EKeys::Gamepad_LeftStick_Up; }
	bool IsDown(const FKey& K) { return K == EKeys::Down || K == EKeys::Gamepad_DPad_Down || K == EKeys::Gamepad_LeftStick_Down; }
	bool IsLeft(const FKey& K) { return K == EKeys::Left || K == EKeys::Gamepad_DPad_Left || K == EKeys::Gamepad_LeftStick_Left; }
	bool IsRight(const FKey& K) { return K == EKeys::Right || K == EKeys::Gamepad_DPad_Right || K == EKeys::Gamepad_LeftStick_Right; }
	bool IsConfirm(const FKey& K) { return K == EKeys::Enter || K == EKeys::SpaceBar || K == EKeys::Gamepad_FaceButton_Bottom; }
	bool IsBack(const FKey& K) { return K == EKeys::Escape || K == EKeys::Gamepad_FaceButton_Right || K == EKeys::Gamepad_Special_Right; }

	int32 Digit(const FKey& K)
	{
		static const FKey Row[] = { EKeys::Zero, EKeys::One, EKeys::Two, EKeys::Three, EKeys::Four, EKeys::Five, EKeys::Six, EKeys::Seven, EKeys::Eight, EKeys::Nine };
		static const FKey Pad[] = { EKeys::NumPadZero, EKeys::NumPadOne, EKeys::NumPadTwo, EKeys::NumPadThree, EKeys::NumPadFour,
			EKeys::NumPadFive, EKeys::NumPadSix, EKeys::NumPadSeven, EKeys::NumPadEight, EKeys::NumPadNine };
		for (int32 i = 0; i < 10; ++i)
		{
			if (K == Row[i] || K == Pad[i]) { return i; }
		}
		return -1;
	}

	struct FVolumeRow { const TCHAR* Label; FSSettings::EFSVolume Category; const TCHAR* ClassPath; };
	const FVolumeRow VolumeRows[] = {
		{ TEXT("Volume général"), FSSettings::EFSVolume::Master, TEXT("/Engine/EngineSounds/Master.Master") },
		{ TEXT("Musique"), FSSettings::EFSVolume::Music, TEXT("/Game/Audio/SC_Musique.SC_Musique") },
		{ TEXT("Effets"), FSSettings::EFSVolume::Effects, TEXT("/Game/Audio/SC_Effets.SC_Effets") },
		{ TEXT("Voix"), FSSettings::EFSVolume::Voice, TEXT("/Game/Audio/SC_Voix.SC_Voix") },
		{ TEXT("Ambiance"), FSSettings::EFSVolume::Ambience, TEXT("/Game/Audio/SC_Ambiance.SC_Ambiance") },
		{ TEXT("Interface"), FSSettings::EFSVolume::Interface, TEXT("/Game/Audio/SC_Interface.SC_Interface") },
	};
	constexpr int32 VolumeCount = UE_ARRAY_COUNT(VolumeRows);

	FString Percent(float V) { return FString::Printf(TEXT("%d %%"), FMath::RoundToInt(100.f * V)); }
	FString YesNo(bool b) { return b ? TEXT("Oui") : TEXT("Non"); }

	const TCHAR* HelpLines[] = {
		TEXT("# Le prologue"),
		TEXT("Mardi, fin septembre, 16 h 25 : tu te promènes avec Ariane sur la place de l'école de Lescoure-Plage."),
		TEXT("Suis l'objectif en haut à gauche. L'horloge en haut à droite avance en temps réel."),
		TEXT("Quand il se passe quelque chose dans la ruelle, un repère « Ruelle » t'indique la direction."),
		TEXT("Pendant l'alerte, tu as quelques secondes et deux actions au plus : photographier, courir, crier, envoyer Ariane."),
		TEXT("Quand le fourgon a disparu, appelle le 17 (touche T ou téléphone)."),
		TEXT("# Se déplacer et regarder"),
		TEXT("ZQSD ou WASD pour marcher, Maj pour courir, la souris pour regarder, la molette pour rapprocher la caméra."),
		TEXT("1, 2, 3 (ou V) changent de vue : épaule, reculée, subjective. L'enquête ne dépend jamais de la vue."),
		TEXT("# Ariane"),
		TEXT("Ariane est libre, sans laisse ni collier. Elle te suit à ton rythme."),
		TEXT("R : au pied · X : reste · G : cherche (tu lances une balle, elle la retrouve)."),
		TEXT("Les bonbons de l'inventaire la font revenir au pied."),
		TEXT("# Parler et ramasser"),
		TEXT("Une invite apparaît quand tu peux agir : E pour parler, répondre ou ramasser."),
		TEXT("Enfile tes gants (inventaire) avant de ramasser une preuve."),
		TEXT("# Carnet, inventaire, téléphone"),
		TEXT("J : carnet d'enquête, tout ce que tu as constaté."),
		TEXT("I : inventaire du sac en bandoulière (téléphone, bonbons, Opinel, gants, lampe) et rubrique Preuves."),
		TEXT("O : téléphone (appeler, répertoire, journal, messages, mails, notifications). L : lampe torche."),
		TEXT("# Sauvegarder"),
		TEXT("F5 : sauvegarde rapide · F9 : la recharger. Menu principal › Sauvegarder : trois emplacements."),
		TEXT("# Menus"),
		TEXT("P (ou Échap hors de l'éditeur, ou Start) : menu principal. Flèches ou croix directionnelle : choisir ; Entrée : valider ;"),
		TEXT("Gauche / Droite : régler ; Échap ou B : revenir. Toutes les touches du clavier se changent dans « Modifier les commandes »."),
		TEXT("# Accessibilité"),
		TEXT("Options : sensibilité et inversion de la souris, taille des sous-titres, temps d'action allongé pendant l'alerte."),
	};
}

// --- Navigation --------------------------------------------------------------------------------

bool AFSHUD::IsActionKey(const TCHAR* ActionName, const FKey& Key) const
{
	TArray<FInputActionKeyMapping> Maps;
	UInputSettings::GetInputSettings()->GetActionMappingByName(FName(ActionName), Maps);
	for (const FInputActionKeyMapping& M : Maps)
	{
		if (M.Key == Key) { return true; }
	}
	return false;
}

void AFSHUD::UpdatePause()
{
	const bool bPause = Page != EFSPage::None && Page != EFSPage::Inventory && Page != EFSPage::Phone;
	UGameplayStatics::SetGamePaused(this, bPause);
}

void AFSHUD::GoTo(EFSPage NewPage)
{
	Stack.Push(Page);
	Page = NewPage;
	Index = 0;
	Scroll = 0;
	Status.Reset();
	bCapturing = false;
	if (NewPage == EFSPage::Phone)
	{
		PhoneScreen = EPhoneScreen::Home;
		PhoneIndex = 0;
		PhoneDial.Reset();
		PhoneLine.Reset();
	}
	UpdatePause();
	PlayUi(TEXT("SFX_UI_Valider"));
}

void AFSHUD::Back()
{
	if (bCapturing)
	{
		bCapturing = false;
		Status = TEXT("Modification annulée.");
		return;
	}
	if (Page == EFSPage::Phone && PhoneScreen != EPhoneScreen::Home)
	{
		PhoneScreen = PhoneScreen == EPhoneScreen::Detail ? EPhoneScreen::Home : EPhoneScreen::Home;
		PhoneIndex = 0;
		PhoneLine.Reset();
		PlayUi(TEXT("SFX_UI_Retour"));
		return;
	}
	if (Stack.Num() == 0)
	{
		return; // écran titre : on ne revient nulle part
	}
	Page = Stack.Pop();
	Index = 0;
	Scroll = 0;
	Status.Reset();
	UpdatePause();
	PlayUi(TEXT("SFX_UI_Retour"));
}

void AFSHUD::OpenPage(EFSPage NewPage)
{
	if (Page == EFSPage::Title)
	{
		return;
	}
	if (Page == NewPage)
	{
		// Même touche : on referme tout.
		Stack.Reset();
		Page = EFSPage::None;
		UpdatePause();
		PlayUi(TEXT("SFX_UI_Retour"));
		return;
	}
	bNotebook = false;
	GoTo(NewPage);
}

void AFSHUD::StartNewGame()
{
	Stack.Reset();
	Page = EFSPage::None;
	UpdatePause();
	IntroStart = Now();
	if (const UFSMissionSubsystem* M = Mission())
	{
		IntroText = M->GetLineText(TEXT("DLG_P_TUTO_01"));
	}
	PlayUi(TEXT("SFX_UI_Valider"));
}

void AFSHUD::OnGameLoaded()
{
	Stack.Reset();
	Page = EFSPage::None;
	UpdatePause();
	Subtitle = FTimedText();
	Hints.Reset();
	Conversation.Reset();
	IntroStart = -1.f;
	bNotebook = false;
}

void AFSHUD::HandleKey(const FKey& Key, bool bRepeat)
{
	// Saisie d'une nouvelle touche pour une commande.
	if (bCapturing)
	{
		if (Key == EKeys::Escape || Key.IsGamepadKey())
		{
			Back();
			return;
		}
		if (!FSKeyBindings::IsAssignable(Key))
		{
			return;
		}
		const TArray<FFSBinding>& All = FSKeyBindings::All();
		if (All.IsValidIndex(Index))
		{
			const FString Swapped = FSKeyBindings::Rebind(All[Index], Key);
			Status = Swapped.IsEmpty()
				? FString::Printf(TEXT("« %s » : %s"), All[Index].Label, *Key.GetDisplayName().ToString())
				: FString::Printf(TEXT("« %s » : %s — échangée avec « %s »"), All[Index].Label, *Key.GetDisplayName().ToString(), *Swapped);
			PlayUi(TEXT("SFX_UI_Valider"));
		}
		bCapturing = false;
		return;
	}

	// Touches qui referment la page qu'elles ont ouverte.
	if (!bRepeat && ((Page == EFSPage::Main && IsActionKey(TEXT("Pause"), Key))
		|| (Page == EFSPage::Inventory && IsActionKey(TEXT("Inventaire"), Key))
		|| (Page == EFSPage::Phone && IsActionKey(TEXT("Telephone"), Key))))
	{
		OpenPage(Page);
		return;
	}

	if (Page == EFSPage::Phone)
	{
		const int32 Vertical = IsUp(Key) ? -1 : (IsDown(Key) ? 1 : 0);
		PhoneKey(Key, Vertical, IsConfirm(Key) || Key == EKeys::E, IsBack(Key));
		return;
	}
	if (IsBack(Key) || (Key == EKeys::BackSpace && Page != EFSPage::Title))
	{
		Back();
		return;
	}
	if (Page == EFSPage::Help)
	{
		const int32 Max = UE_ARRAY_COUNT(HelpLines) - 1;
		if (IsUp(Key)) { Scroll = FMath::Max(0, Scroll - 1); }
		if (IsDown(Key)) { Scroll = FMath::Min(Max, Scroll + 1); }
		if (IsConfirm(Key)) { Back(); }
		return;
	}

	const int32 Count = Rows(Page).Num();
	if (Count == 0)
	{
		return;
	}
	if (IsUp(Key)) { Index = (Index - 1 + Count) % Count; PlayUi(TEXT("SFX_UI_Deplacer")); }
	else if (IsDown(Key)) { Index = (Index + 1) % Count; PlayUi(TEXT("SFX_UI_Deplacer")); }
	else if (IsLeft(Key)) { Adjust(Index, -1); }
	else if (IsRight(Key)) { Adjust(Index, 1); }
	else if (IsConfirm(Key) || (Page == EFSPage::Inventory && Key == EKeys::E)) { if (!bRepeat) { Activate(Index); } }
}

// --- Contenu des pages -------------------------------------------------------------------------

TArray<FName> AFSHUD::EvidenceIds() const
{
	TArray<FName> Out;
	if (const UFSMissionSubsystem* M = Mission())
	{
		for (const FName& Id : M->GetAcquiredClues())
		{
			const FString Kind = M->GetClueKind(Id);
			if (Kind == TEXT("objet") || Kind == TEXT("document")) { Out.Add(Id); }
		}
	}
	return Out;
}

TArray<AFSHUD::FMenuRow> AFSHUD::Rows(EFSPage ForPage)
{
	TArray<FMenuRow> R;
	switch (ForPage)
	{
	case EFSPage::Title:
		R = { {TEXT("Nouvelle partie"), TEXT("")}, {TEXT("Charger une partie"), TEXT("")}, {TEXT("Modifier les commandes"), TEXT("")},
			{TEXT("Son"), TEXT("")}, {TEXT("Options"), TEXT("")}, {TEXT("Aide"), TEXT("")}, {TEXT("Quitter le jeu"), TEXT("")} };
		break;
	case EFSPage::Main:
		R = { {TEXT("Retour au jeu"), TEXT("")}, {TEXT("Sauvegarder"), TEXT("")}, {TEXT("Charger une partie"), TEXT("")},
			{TEXT("Modifier les commandes"), TEXT("")}, {TEXT("Son"), TEXT("")}, {TEXT("Options"), TEXT("")}, {TEXT("Aide"), TEXT("")},
			{TEXT("Recommencer le prologue"), TEXT("")}, {TEXT("Quitter le jeu"), TEXT("")} };
		break;
	case EFSPage::Controls:
		for (const FFSBinding& B : FSKeyBindings::All()) { R.Add({ B.Label, FSKeyBindings::KeysText(B) }); }
		R.Add({ TEXT("Rétablir les commandes d'origine"), TEXT("") });
		R.Add({ TEXT("Retour"), TEXT("") });
		break;
	case EFSPage::Sound:
		for (const FVolumeRow& V : VolumeRows) { R.Add({ V.Label, Percent(FSSettings::Volume(V.Category)) }); }
		R.Add({ TEXT("Couper le son"), YesNo(FSSettings::Muted()) });
		R.Add({ TEXT("Tester le son"), TEXT("") });
		R.Add({ TEXT("Retour"), TEXT("") });
		break;
	case EFSPage::Options:
		R = { {TEXT("Sensibilité de la souris"), FString::Printf(TEXT("%.2f"), FSSettings::MouseSensitivity())},
			{TEXT("Inverser l'axe vertical"), YesNo(FSSettings::InvertY())},
			{TEXT("Taille des sous-titres"), Percent(FSSettings::SubtitleScale())},
			{TEXT("Temps d'action allongé (alerte)"), YesNo(FSSettings::ExtendedActionTime())},
			{TEXT("Retour"), TEXT("")} };
		break;
	case EFSPage::SaveSlots:
	case EFSPage::LoadSlots:
		for (int32 Slot = 1; Slot <= 3; ++Slot)
		{
			const FString D = UFSSaveGame::Describe(Slot);
			R.Add({ FString::Printf(TEXT("Emplacement %d"), Slot), D.IsEmpty() ? FString(TEXT("libre")) : D });
		}
		{
			const FString D = UFSSaveGame::Describe(0);
			R.Add({ TEXT("Sauvegarde rapide (F5)"), D.IsEmpty() ? FString(TEXT("libre")) : D });
		}
		R.Add({ TEXT("Retour"), TEXT("") });
		break;
	case EFSPage::Inventory:
		if (const AFSHeroCharacter* H = Hero())
		{
			for (const FFSBagItem& Item : AFSHeroCharacter::BagItems()) { R.Add({ Item.Name, H->BagItemState(Item.Id) }); }
		}
		if (const UFSMissionSubsystem* M = Mission())
		{
			for (const FName& Id : EvidenceIds()) { R.Add({ M->GetClueName(Id), TEXT("preuve") }); }
		}
		break;
	default:
		break;
	}
	return R;
}

void AFSHUD::Adjust(int32 Row, int32 Direction)
{
	switch (Page)
	{
	case EFSPage::Sound:
		if (Row < VolumeCount)
		{
			FSSettings::SetVolume(VolumeRows[Row].Category, FMath::RoundToFloat((FSSettings::Volume(VolumeRows[Row].Category) + 0.1f * Direction) * 10.f) / 10.f);
			ApplyVolumes();
			PlayUi(TEXT("SFX_UI_Deplacer"));
		}
		else if (Row == VolumeCount)
		{
			FSSettings::SetMuted(!FSSettings::Muted());
			ApplyVolumes();
		}
		break;
	case EFSPage::Options:
		switch (Row)
		{
		case 0: FSSettings::SetMouseSensitivity(FSSettings::MouseSensitivity() + 0.25f * Direction); break;
		case 1: FSSettings::SetInvertY(!FSSettings::InvertY()); break;
		case 2:
		{
			const float Next = FSSettings::SubtitleScale() + 0.25f * Direction;
			FSSettings::SetSubtitleScale(Next > 1.51f ? 1.f : (Next < 0.99f ? 1.5f : Next));
			break;
		}
		case 3: FSSettings::SetExtendedActionTime(!FSSettings::ExtendedActionTime()); break;
		default: break;
		}
		PlayUi(TEXT("SFX_UI_Deplacer"));
		break;
	default:
		break;
	}
}

void AFSHUD::Activate(int32 Row)
{
	const int32 Count = Rows(Page).Num();
	const bool bLast = Row == Count - 1;
	switch (Page)
	{
	case EFSPage::Title:
		switch (Row)
		{
		case 0: StartNewGame(); break;
		case 1: GoTo(EFSPage::LoadSlots); break;
		case 2: GoTo(EFSPage::Controls); break;
		case 3: GoTo(EFSPage::Sound); break;
		case 4: GoTo(EFSPage::Options); break;
		case 5: GoTo(EFSPage::Help); break;
		default: UKismetSystemLibrary::QuitGame(this, GetOwningPlayerController(), EQuitPreference::Quit, false); break;
		}
		break;
	case EFSPage::Main:
		switch (Row)
		{
		case 0: OpenPage(EFSPage::Main); break;
		case 1: GoTo(EFSPage::SaveSlots); break;
		case 2: GoTo(EFSPage::LoadSlots); break;
		case 3: GoTo(EFSPage::Controls); break;
		case 4: GoTo(EFSPage::Sound); break;
		case 5: GoTo(EFSPage::Options); break;
		case 6: GoTo(EFSPage::Help); break;
		case 7:
			UGameplayStatics::SetGamePaused(this, false);
			UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
			break;
		default: UKismetSystemLibrary::QuitGame(this, GetOwningPlayerController(), EQuitPreference::Quit, false); break;
		}
		break;
	case EFSPage::Controls:
		if (bLast) { Back(); }
		else if (Row == Count - 2)
		{
			FSKeyBindings::ResetDefaults();
			Status = TEXT("Commandes d'origine rétablies.");
			PlayUi(TEXT("SFX_UI_Valider"));
		}
		else
		{
			bCapturing = true;
			Status.Reset();
		}
		break;
	case EFSPage::Sound:
		if (bLast) { Back(); }
		else if (Row == VolumeCount + 1) { PlayUi(TEXT("SFX_Notification")); Status = TEXT("Son de test joué (catégorie Interface)."); }
		else { Adjust(Row, 1); }
		break;
	case EFSPage::Options:
		if (bLast) { Back(); } else { Adjust(Row, 1); }
		break;
	case EFSPage::SaveSlots:
		if (bLast) { Back(); break; }
		Status = UFSSaveGame::SaveSlot(this, Row == 3 ? 0 : Row + 1) ? TEXT("Partie enregistrée.") : TEXT("Échec de l'enregistrement.");
		PlayUi(TEXT("SFX_UI_Valider"));
		break;
	case EFSPage::LoadSlots:
	{
		if (bLast) { Back(); break; }
		const bool bFromTitle = Stack.Num() > 0 && Stack[0] == EFSPage::Title;
		if (UFSSaveGame::LoadSlot(this, Row == 3 ? 0 : Row + 1))
		{
			OnGameLoaded();
			ShowToast(TEXT("Partie chargée"), 2.5f);
			PlayUi(TEXT("SFX_UI_Valider"));
		}
		else
		{
			Status = bFromTitle ? TEXT("Emplacement vide ou incompatible.") : TEXT("Rien à charger dans cet emplacement.");
		}
		break;
	}
	case EFSPage::Inventory:
		if (AFSHeroCharacter* H = Hero())
		{
			const TArray<FFSBagItem>& Bag = AFSHeroCharacter::BagItems();
			if (Row < Bag.Num())
			{
				const FString Id = Bag[Row].Id;
				if (Id == TEXT("TELEPHONE"))
				{
					GoTo(EFSPage::Phone);
					return;
				}
				Status = H->UseBagItem(Id);
				PlayUi(TEXT("SFX_UI_Valider"));
			}
			else
			{
				Status.Reset(); // la fiche de la preuve est déjà affichée à droite
			}
		}
		break;
	default:
		break;
	}
}

// --- Son ---------------------------------------------------------------------------------------

void AFSHUD::ApplyVolumes()
{
	if (!Mix)
	{
		Mix = NewObject<USoundMix>(this);
	}
	const float Master = FSSettings::Muted() ? 0.f : FSSettings::Volume(FSSettings::EFSVolume::Master);
	for (int32 i = 0; i < VolumeCount; ++i)
	{
		if (USoundClass* Class = LoadObject<USoundClass>(nullptr, VolumeRows[i].ClassPath, nullptr, LOAD_NoWarn | LOAD_Quiet))
		{
			// Catégories : volume général × volume de la catégorie ; classe Master : sons sans catégorie.
			const float V = i == 0 ? Master : Master * FSSettings::Volume(VolumeRows[i].Category);
			UGameplayStatics::SetSoundMixClassOverride(this, Mix, Class, V, 1.f, 0.f, false);
		}
	}
	if (!bMixPushed)
	{
		UGameplayStatics::PushSoundMixModifier(this, Mix);
		bMixPushed = true;
	}
}

void AFSHUD::PlayUi(const TCHAR* SoundName)
{
	const FString Path = FString::Printf(TEXT("/Game/Audio/%s.%s"), SoundName, SoundName);
	if (USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet))
	{
		UGameplayStatics::PlaySound2D(this, Sound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
}

// --- Téléphone ---------------------------------------------------------------------------------

void AFSHUD::PhoneCall(const FString& Number)
{
	UFSPhoneSubsystem* P = Phone();
	if (!P || Number.IsEmpty())
	{
		return;
	}
	bool bEmergency = false;
	const FString Result = P->Call(Number, ClockText(), bEmergency);
	const FString Who = P->ContactName(Number);
	PlayUi(TEXT("SFX_UI_Valider"));
	if (!bEmergency)
	{
		PhoneLine = FString::Printf(TEXT("Appel : %s\n%s"), *Who, *Result);
		ShowLine(Who, Result, 6.f);
		return;
	}
	AFSPrologueDirector* D = Director();
	const int32 Phase = D ? D->GetPhaseIndex() : 0;
	if (Phase == 4 && D)
	{
		// Le fourgon a disparu : l'appel au 17 termine le prologue (comme la touche T).
		Stack.Reset();
		Page = EFSPage::None;
		UpdatePause();
		D->CallPolice();
		return;
	}
	PhoneLine = Phase == 0 ? P->CallRule(TEXT("emergency_before"))
		: Phase >= 5 ? P->CallRule(TEXT("already_called"))
		: P->CallRule(TEXT("busy_after_alert"));
}

void AFSHUD::PhoneKey(const FKey& Key, int32 Vertical, bool bConfirm, bool bBack)
{
	UFSPhoneSubsystem* P = Phone();
	if (!P)
	{
		if (bBack) { OpenPage(EFSPage::Phone); }
		return;
	}
	if (PhoneScreen == EPhoneScreen::Dial)
	{
		const int32 D = Digit(Key);
		if (D >= 0 && PhoneDial.Len() < 14) { PhoneDial.AppendChar(static_cast<TCHAR>(TEXT('0') + D)); PlayUi(TEXT("SFX_UI_Deplacer")); return; }
		if (Key == EKeys::BackSpace) { PhoneDial.LeftChopInline(1); return; }
		if (bConfirm) { PhoneCall(PhoneDial); return; }
	}
	if (bBack || (Key == EKeys::BackSpace && PhoneScreen != EPhoneScreen::Dial))
	{
		if (PhoneScreen == EPhoneScreen::Home) { Back(); }
		else if (PhoneScreen == EPhoneScreen::Detail) { PhoneScreen = PhoneDetail.ToString().StartsWith(TEXT("MAIL")) ? EPhoneScreen::Mails
			: (PhoneDetail.ToString().StartsWith(TEXT("NOTIF")) ? EPhoneScreen::Notifications : EPhoneScreen::Messages); }
		else { PhoneScreen = EPhoneScreen::Home; PhoneIndex = 0; PhoneLine.Reset(); }
		PlayUi(TEXT("SFX_UI_Retour"));
		return;
	}

	int32 Count = 0;
	switch (PhoneScreen)
	{
	case EPhoneScreen::Home: Count = 6; break;
	case EPhoneScreen::Contacts: Count = P->GetContacts().Num(); break;
	case EPhoneScreen::Log: Count = P->GetItems(TEXT("call")).Num(); break;
	case EPhoneScreen::Messages: Count = P->GetItems(TEXT("message")).Num(); break;
	case EPhoneScreen::Mails: Count = P->GetItems(TEXT("mail")).Num(); break;
	case EPhoneScreen::Notifications: Count = P->GetItems(TEXT("notification")).Num(); break;
	default: break;
	}
	if (Vertical != 0 && Count > 0)
	{
		PhoneIndex = (PhoneIndex + Vertical + Count) % Count;
		PlayUi(TEXT("SFX_UI_Deplacer"));
		return;
	}
	if (!bConfirm)
	{
		return;
	}
	switch (PhoneScreen)
	{
	case EPhoneScreen::Home:
	{
		static const EPhoneScreen Apps[] = { EPhoneScreen::Dial, EPhoneScreen::Contacts, EPhoneScreen::Log,
			EPhoneScreen::Messages, EPhoneScreen::Mails, EPhoneScreen::Notifications };
		PhoneScreen = Apps[FMath::Clamp(PhoneIndex, 0, 5)];
		PhoneIndex = 0;
		PhoneDial.Reset();
		PhoneLine.Reset();
		PlayUi(TEXT("SFX_UI_Valider"));
		break;
	}
	case EPhoneScreen::Contacts:
		if (P->GetContacts().IsValidIndex(PhoneIndex)) { PhoneCall(P->GetContacts()[PhoneIndex].Number); }
		break;
	case EPhoneScreen::Log:
	{
		TArray<FFSPhoneItem*> Calls = P->GetItems(TEXT("call"));
		if (Calls.IsValidIndex(PhoneIndex)) { PhoneCall(Calls[PhoneIndex]->Body); }
		break;
	}
	case EPhoneScreen::Messages:
	case EPhoneScreen::Mails:
	case EPhoneScreen::Notifications:
	{
		const TCHAR* Kind = PhoneScreen == EPhoneScreen::Messages ? TEXT("message") : (PhoneScreen == EPhoneScreen::Mails ? TEXT("mail") : TEXT("notification"));
		TArray<FFSPhoneItem*> List = P->GetItems(Kind);
		if (List.IsValidIndex(PhoneIndex))
		{
			PhoneDetail = List[PhoneIndex]->Id;
			P->MarkRead(PhoneDetail);
			PhoneScreen = EPhoneScreen::Detail;
			PlayUi(TEXT("SFX_UI_Valider"));
		}
		break;
	}
	default:
		break;
	}
}

// --- Dessin des pages ----------------------------------------------------------------------------

void AFSHUD::DrawPage()
{
	switch (Page)
	{
	case EFSPage::Title:
	{
		Box(0.f, 0.f, Canvas->ClipX, Canvas->ClipY, FLinearColor(0.02f, 0.02f, 0.03f, 0.94f));
		const float U = Ui();
		Text(UIText(TEXT("UI_TITRE_JEU"), TEXT("Faux-semblants")), 0.5f * Canvas->ClipX, Canvas->ClipY * 0.14f, GEngine->GetLargeFont(), 3.0f * U, Cream, true);
		Text(UIText(TEXT("UI_TITRE_SOUS"), TEXT("Prologue")), 0.5f * Canvas->ClipX, Canvas->ClipY * 0.14f + 110.f * U, GEngine->GetMediumFont(), 1.5f * U, Amber, true);
		DrawMenuList(FString(), Rows(Page), TEXT("Flèches : choisir · Entrée : valider"), false);
		break;
	}
	case EFSPage::Main: DrawMenuList(TEXT("Menu principal"), Rows(Page), TEXT("Flèches : choisir · Entrée : valider · Échap ou P : retour au jeu"), true); break;
	case EFSPage::Controls:
		DrawMenuList(TEXT("Modifier les commandes"), Rows(Page), bCapturing
			? FString::Printf(TEXT("Appuie sur la nouvelle touche pour « %s » (Échap : annuler)"), FSKeyBindings::All().IsValidIndex(Index) ? FSKeyBindings::All()[Index].Label : TEXT(""))
			: FString(TEXT("Entrée : changer la touche · Échap : retour · La manette garde ses boutons")), true);
		break;
	case EFSPage::Sound: DrawMenuList(TEXT("Son"), Rows(Page), TEXT("Gauche / Droite : régler · Entrée : valider · Échap : retour"), true); break;
	case EFSPage::Options: DrawMenuList(TEXT("Options"), Rows(Page), TEXT("Gauche / Droite : régler · Échap : retour"), true); break;
	case EFSPage::SaveSlots: DrawMenuList(TEXT("Sauvegarder"), Rows(Page), TEXT("Entrée : enregistrer dans l'emplacement · Échap : retour"), true); break;
	case EFSPage::LoadSlots: DrawMenuList(TEXT("Charger une partie"), Rows(Page), TEXT("Entrée : charger · Échap : retour"), true); break;
	case EFSPage::Help: DrawHelp(); break;
	case EFSPage::Inventory: DrawInventory(); break;
	case EFSPage::Phone: DrawPhone(); break;
	default: break;
	}
}

void AFSHUD::DrawMenuList(const FString& Title, const TArray<FMenuRow>& List, const FString& Footer, bool bWithBackdrop)
{
	const float U = Ui();
	if (bWithBackdrop)
	{
		Box(0.f, 0.f, Canvas->ClipX, Canvas->ClipY, FLinearColor(0.f, 0.f, 0.f, 0.72f));
	}
	UFont* Large = GEngine->GetLargeFont();
	UFont* Medium = GEngine->GetMediumFont();
	UFont* Small = GEngine->GetSmallFont();
	const float W = FMath::Min(1000.f * U, Canvas->ClipX - 80.f * U);
	const float X = 0.5f * (Canvas->ClipX - W);
	float Y = Title.IsEmpty() ? Canvas->ClipY * 0.36f : Canvas->ClipY * 0.10f;
	if (!Title.IsEmpty())
	{
		Text(Title, X, Y, Large, 1.8f * U, Cream);
		Y += 80.f * U;
	}
	const float RowH = 42.f * U;
	const int32 Visible = FMath::Max(3, FMath::FloorToInt((Canvas->ClipY * 0.86f - Y - 90.f * U) / RowH));
	if (Index < Scroll) { Scroll = Index; }
	if (Index >= Scroll + Visible) { Scroll = Index - Visible + 1; }
	Scroll = FMath::Clamp(Scroll, 0, FMath::Max(0, List.Num() - Visible));
	for (int32 i = Scroll; i < List.Num() && i < Scroll + Visible; ++i)
	{
		const bool bSel = i == Index;
		if (bSel)
		{
			Box(X - 14.f * U, Y - 5.f * U, W + 28.f * U, RowH - 4.f * U, FLinearColor(1.f, 0.72f, 0.28f, bCapturing ? 0.45f : 0.22f));
			Box(X - 14.f * U, Y - 5.f * U, 5.f * U, RowH - 4.f * U, Amber);
		}
		Text(List[i].Label, X, Y, Medium, 1.1f * U, bSel ? Cream : Soft);
		if (!List[i].Value.IsEmpty())
		{
			const FString V = (bSel && Page != EFSPage::SaveSlots && Page != EFSPage::LoadSlots && Page != EFSPage::Controls && Page != EFSPage::Inventory)
				? FString::Printf(TEXT("‹  %s  ›"), *List[i].Value) : List[i].Value;
			const float VW = Measure(V, Medium, 1.05f * U).X;
			Text(V, X + W - VW, Y, Medium, 1.05f * U, bSel ? Amber : Dim);
		}
		Y += RowH;
	}
	if (List.Num() > Visible)
	{
		Text(FString::Printf(TEXT("%d / %d"), Index + 1, List.Num()), X + W - 60.f * U, Y + 4.f * U, Small, 1.f * U, Dim);
	}
	if (!Status.IsEmpty())
	{
		Text(Status, X, Y + 20.f * U, Medium, 1.0f * U, Amber);
	}
	Text(Footer, 0.5f * Canvas->ClipX, Canvas->ClipY - 60.f * U, Small, 1.05f * U, Soft, true);
}

void AFSHUD::DrawHelp()
{
	const float U = Ui();
	Box(0.f, 0.f, Canvas->ClipX, Canvas->ClipY, FLinearColor(0.f, 0.f, 0.f, 0.8f));
	UFont* Medium = GEngine->GetMediumFont();
	const float W = FMath::Min(1200.f * U, Canvas->ClipX - 80.f * U);
	const float X = 0.5f * (Canvas->ClipX - W);
	float Y = Canvas->ClipY * 0.08f;
	Text(TEXT("Aide"), X, Y, GEngine->GetLargeFont(), 1.8f * U, Cream);
	Y += 80.f * U;
	const float LineH = Measure(TEXT("Ag"), Medium, 1.0f * U).Y;
	for (int32 i = Scroll; i < static_cast<int32>(UE_ARRAY_COUNT(HelpLines)); ++i)
	{
		FString Line = HelpLines[i];
		const bool bHeading = Line.StartsWith(TEXT("# "));
		if (bHeading) { Line.RightChopInline(2); Y += 10.f * U; }
		for (const FString& L : Wrap(Line, Medium, bHeading ? 1.15f * U : 1.0f * U, W))
		{
			if (Y > Canvas->ClipY - 110.f * U) { break; }
			Text(L, X, Y, Medium, bHeading ? 1.15f * U : 1.0f * U, bHeading ? Amber : Soft);
			Y += LineH * (bHeading ? 1.2f : 1.f);
		}
	}
	Text(TEXT("Haut / Bas : faire défiler · Échap ou Entrée : retour"), 0.5f * Canvas->ClipX, Canvas->ClipY - 60.f * U, GEngine->GetSmallFont(), 1.05f * U, Soft, true);
}

void AFSHUD::DrawInventory()
{
	const float U = Ui();
	UFont* Large = GEngine->GetLargeFont();
	UFont* Medium = GEngine->GetMediumFont();
	UFont* Small = GEngine->GetSmallFont();
	const float W = FMath::Min(1200.f * U, Canvas->ClipX - 60.f * U);
	const float H = FMath::Min(700.f * U, Canvas->ClipY - 120.f * U);
	const float X0 = 0.5f * (Canvas->ClipX - W);
	const float Y0 = 0.5f * (Canvas->ClipY - H);
	Box(X0, Y0, W, H, Panel);
	Box(X0, Y0, W, 6.f * U, Amber);
	Text(TEXT("Sac en bandoulière"), X0 + 30.f * U, Y0 + 22.f * U, Large, 1.3f * U, Cream);

	const TArray<FMenuRow> List = Rows(EFSPage::Inventory);
	const TArray<FFSBagItem>& Bag = AFSHeroCharacter::BagItems();
	const TArray<FName> Evidence = EvidenceIds();
	Index = FMath::Clamp(Index, 0, FMath::Max(0, List.Num() - 1));
	const float ListW = 0.45f * W;
	float Y = Y0 + 80.f * U;
	const float RowH = 38.f * U;
	for (int32 i = 0; i < List.Num(); ++i)
	{
		if (i == Bag.Num())
		{
			Y += 12.f * U;
			Text(TEXT("PREUVES"), X0 + 30.f * U, Y, Small, 1.0f * U, Amber);
			Y += 28.f * U;
		}
		if (Y > Y0 + H - 60.f * U) { break; }
		const bool bSel = i == Index;
		if (bSel) { Box(X0 + 18.f * U, Y - 4.f * U, ListW, RowH - 4.f * U, FLinearColor(1.f, 0.72f, 0.28f, 0.22f)); }
		Text(List[i].Label, X0 + 30.f * U, Y, Medium, 1.0f * U, bSel ? Cream : Soft);
		if (!List[i].Value.IsEmpty() && i < Bag.Num())
		{
			const float VW = Measure(List[i].Value, Small, 0.95f * U).X;
			Text(List[i].Value, X0 + 18.f * U + ListW - VW - 12.f * U, Y + 4.f * U, Small, 0.95f * U, Amber);
		}
		Y += RowH;
	}
	if (Evidence.Num() == 0)
	{
		Y += 12.f * U;
		Text(TEXT("PREUVES"), X0 + 30.f * U, Y, Small, 1.0f * U, Amber);
		Text(TEXT("Aucune preuve pour l'instant."), X0 + 30.f * U, Y + 28.f * U, Medium, 0.95f * U, Dim);
	}

	// Fiche de l'objet sélectionné.
	const float DX = X0 + ListW + 50.f * U;
	const float DW = W - ListW - 80.f * U;
	float DY = Y0 + 80.f * U;
	FString Name, Body;
	const UFSMissionSubsystem* M = Mission();
	if (Index < Bag.Num())
	{
		Name = Bag[Index].Name;
		Body = Bag[Index].Description;
	}
	else if (M && Evidence.IsValidIndex(Index - Bag.Num()))
	{
		Name = M->GetClueName(Evidence[Index - Bag.Num()]);
		Body = M->GetClueFact(Evidence[Index - Bag.Num()]);
	}
	Text(Name, DX, DY, Medium, 1.2f * U, Cream);
	DY += 50.f * U;
	const float LineH = Measure(TEXT("Ag"), Medium, 0.95f * U).Y;
	for (const FString& L : Wrap(Body, Medium, 0.95f * U, DW))
	{
		if (DY > Y0 + H - 120.f * U) { break; }
		Text(L, DX, DY, Medium, 0.95f * U, Soft);
		DY += LineH;
	}
	if (!Status.IsEmpty())
	{
		DY += 20.f * U;
		for (const FString& L : Wrap(Status, Medium, 0.95f * U, DW))
		{
			Text(L, DX, DY, Medium, 0.95f * U, Amber);
			DY += LineH;
		}
	}
	Text(Index < Bag.Num() ? TEXT("Entrée ou E : utiliser · Échap ou I : fermer") : TEXT("Échap ou I : fermer"),
		X0 + W - 30.f * U - Measure(TEXT("Entrée ou E : utiliser · Échap ou I : fermer"), Small, 1.f * U).X, Y0 + H - 36.f * U, Small, 1.f * U, Soft);
}

void AFSHUD::DrawPhone()
{
	UFSPhoneSubsystem* P = Phone();
	const float U = Ui();
	UFont* Medium = GEngine->GetMediumFont();
	UFont* Small = GEngine->GetSmallFont();
	const float W = 420.f * U;
	const float H = FMath::Min(760.f * U, Canvas->ClipY - 80.f * U);
	const float X0 = Canvas->ClipX - W - 60.f * U;
	const float Y0 = 0.5f * (Canvas->ClipY - H);
	Box(X0 - 10.f * U, Y0 - 10.f * U, W + 20.f * U, H + 20.f * U, FLinearColor(0.02f, 0.02f, 0.02f, 0.98f));
	Box(X0, Y0, W, H, FLinearColor(0.10f, 0.12f, 0.16f, 0.98f));
	// Barre d'état : heure, réseau, batterie (20 % après l'alerte batterie).
	const bool bLowBattery = P && P->GetItems(TEXT("notification")).ContainsByPredicate([](const FFSPhoneItem* I) { return I->Id == FName(TEXT("NOTIF_BATTERIE")); });
	Text(ClockText(), X0 + 16.f * U, Y0 + 10.f * U, Small, 1.f * U, Cream);
	const FString Right = FString::Printf(TEXT("4G  %s"), bLowBattery ? TEXT("20 %") : TEXT("64 %"));
	Text(Right, X0 + W - 16.f * U - Measure(Right, Small, 1.f * U).X, Y0 + 10.f * U, Small, 1.f * U, bLowBattery ? Amber : Cream);
	float Y = Y0 + 50.f * U;
	const float RowH = 52.f * U;
	const float LineH = Measure(TEXT("Ag"), Medium, 0.9f * U).Y;
	auto Title = [&](const FString& S) { Text(S, X0 + 20.f * U, Y, Medium, 1.2f * U, Cream); Y += 50.f * U; };
	auto Row = [&](int32 i, const FString& Main, const FString& Sub, bool bUnread)
	{
		if (Y > Y0 + H - 90.f * U) { return; }
		if (i == PhoneIndex) { Box(X0 + 8.f * U, Y - 4.f * U, W - 16.f * U, RowH - 4.f * U, FLinearColor(1.f, 0.72f, 0.28f, 0.22f)); }
		Text(Main, X0 + 20.f * U, Y, Medium, 0.95f * U, bUnread ? Amber : Cream);
		Text(Sub, X0 + 20.f * U, Y + 24.f * U, Small, 0.85f * U, Dim);
		Y += RowH;
	};
	if (!P)
	{
		Text(TEXT("Téléphone indisponible"), X0 + 20.f * U, Y, Medium, 1.f * U, Soft);
		return;
	}
	switch (PhoneScreen)
	{
	case EPhoneScreen::Home:
	{
		Title(TEXT("Téléphone"));
		const int32 NMsg = P->CountUnread(TEXT("message")), NMail = P->CountUnread(TEXT("mail")), NNotif = P->CountUnread(TEXT("notification"));
		auto Badge = [](int32 N) { return N > 0 ? FString::Printf(TEXT("%d non lu%s"), N, N > 1 ? TEXT("s") : TEXT("")) : FString(); };
		Row(0, TEXT("Appeler (clavier)"), TEXT("Composer un numéro"), false);
		Row(1, TEXT("Répertoire"), FString::Printf(TEXT("%d contacts"), P->GetContacts().Num()), false);
		Row(2, TEXT("Journal d'appels"), FString::Printf(TEXT("%d appels"), P->GetItems(TEXT("call")).Num()), false);
		Row(3, TEXT("Messages"), Badge(NMsg), NMsg > 0);
		Row(4, TEXT("Mails"), Badge(NMail), NMail > 0);
		Row(5, TEXT("Notifications"), Badge(NNotif), NNotif > 0);
		break;
	}
	case EPhoneScreen::Dial:
	{
		Title(TEXT("Appeler"));
		const FString Shown = PhoneDial.IsEmpty() ? FString(TEXT("_")) : PhoneDial;
		Text(Shown, X0 + 0.5f * W, Y + 20.f * U, GEngine->GetLargeFont(), 1.6f * U, Cream, true);
		Y += 110.f * U;
		const FString Who = PhoneDial.IsEmpty() ? FString() : P->ContactName(PhoneDial);
		if (!Who.IsEmpty() && Who != PhoneDial) { Text(Who, X0 + 0.5f * W, Y, Medium, 0.95f * U, Amber, true); }
		Y += 50.f * U;
		Text(TEXT("Chiffres : composer · Retour arrière : effacer"), X0 + 0.5f * W, Y, Small, 0.9f * U, Dim, true);
		Y += 30.f * U;
		Text(TEXT("Entrée : appeler"), X0 + 0.5f * W, Y, Small, 0.9f * U, Dim, true);
		Y += 50.f * U;
		break;
	}
	case EPhoneScreen::Contacts:
	{
		Title(TEXT("Répertoire"));
		const TArray<FFSPhoneContact>& C = P->GetContacts();
		for (int32 i = 0; i < C.Num(); ++i) { Row(i, C[i].Name, C[i].Number, C[i].bEmergency); }
		break;
	}
	case EPhoneScreen::Log:
	{
		Title(TEXT("Journal d'appels"));
		TArray<FFSPhoneItem*> Calls = P->GetItems(TEXT("call"));
		for (int32 i = 0; i < Calls.Num(); ++i) { Row(i, Calls[i]->From, FString::Printf(TEXT("%s · %s"), *Calls[i]->Title, *Calls[i]->At), false); }
		break;
	}
	case EPhoneScreen::Messages:
	case EPhoneScreen::Mails:
	case EPhoneScreen::Notifications:
	{
		const bool bMsg = PhoneScreen == EPhoneScreen::Messages, bMail = PhoneScreen == EPhoneScreen::Mails;
		Title(bMsg ? TEXT("Messages") : (bMail ? TEXT("Mails") : TEXT("Notifications")));
		TArray<FFSPhoneItem*> List = P->GetItems(bMsg ? TEXT("message") : (bMail ? TEXT("mail") : TEXT("notification")));
		if (List.Num() == 0) { Text(TEXT("Rien pour l'instant."), X0 + 20.f * U, Y, Medium, 0.9f * U, Dim); }
		for (int32 i = 0; i < List.Num(); ++i)
		{
			const FString Main = bMail ? List[i]->Title : List[i]->From;
			FString Preview = bMail ? List[i]->From : List[i]->Body;
			if (Preview.Len() > 34) { Preview = Preview.Left(33) + TEXT("…"); }
			Row(i, Main, FString::Printf(TEXT("%s · %s"), *List[i]->At, *Preview), !List[i]->bRead);
		}
		break;
	}
	case EPhoneScreen::Detail:
	{
		static const TCHAR* Kinds[] = { TEXT("message"), TEXT("mail"), TEXT("notification") };
		for (const TCHAR* Kind : Kinds)
		{
			for (FFSPhoneItem* I : P->GetItems(Kind))
			{
				if (I->Id != PhoneDetail) { continue; }
				Title(I->Title.IsEmpty() ? I->From : I->Title);
				Text(FString::Printf(TEXT("%s · %s"), *I->From, *I->At), X0 + 20.f * U, Y, Small, 0.9f * U, Dim);
				Y += 40.f * U;
				for (const FString& L : Wrap(I->Body, Medium, 0.9f * U, W - 40.f * U))
				{
					if (Y > Y0 + H - 80.f * U) { break; }
					Text(L, X0 + 20.f * U, Y, Medium, 0.9f * U, Cream);
					Y += LineH;
				}
			}
		}
		break;
	}
	default:
		break;
	}
	if (!PhoneLine.IsEmpty())
	{
		TArray<FString> Parts;
		PhoneLine.ParseIntoArray(Parts, TEXT("\n"));
		float LY = Y0 + H - 90.f * U - LineH * 2.f;
		Box(X0 + 8.f * U, LY - 8.f * U, W - 16.f * U, Y0 + H - 60.f * U - LY, FLinearColor(0.f, 0.f, 0.f, 0.5f));
		for (const FString& Part : Parts)
		{
			for (const FString& L : Wrap(Part, Small, 0.9f * U, W - 40.f * U))
			{
				if (LY > Y0 + H - 70.f * U) { break; }
				Text(L, X0 + 20.f * U, LY, Small, 0.9f * U, Amber);
				LY += Measure(TEXT("Ag"), Small, 0.9f * U).Y;
			}
		}
	}
	Text(TEXT("Flèches · Entrée · Échap : retour · O : ranger"), X0 + 0.5f * W, Y0 + H - 34.f * U, Small, 0.85f * U, Dim, true);
}
