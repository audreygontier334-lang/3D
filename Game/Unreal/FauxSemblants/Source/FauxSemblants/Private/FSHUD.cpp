#include "FSHUD.h"
#include "FauxSemblants.h"
#include "FSMissionSubsystem.h"
#include "FSPrologueDirector.h"
#include "FSHeroCharacter.h"
#include "FSSettings.h"
#include "FSPhoneSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	// Phases de AFSPrologueDirector::GetPhaseIndex().
	enum : int32 { PhaseBefore = 0, PhaseWindow = 1, PhaseHolding = 2, PhaseDeparting = 3, PhaseGone = 4, PhaseChapter1 = 5 };

	const FLinearColor Cream(1.f, 0.93f, 0.80f, 1.f);
	const FLinearColor Amber(1.f, 0.72f, 0.28f, 1.f);
	const FLinearColor Soft(0.85f, 0.85f, 0.85f, 1.f);
	const FLinearColor Shade(0.f, 0.f, 0.f, 0.55f);
	const float IntroSeconds = 6.f;
}

AFSHUD* AFSHUD::Get(const UObject* WorldContext)
{
	const APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, 0);
	return PC ? Cast<AFSHUD>(PC->GetHUD()) : nullptr;
}

void AFSHUD::BeginPlay()
{
	Super::BeginPlay();
	// Écran titre : le prologue (horloge, figurants) attend que la joueuse choisisse « Nouvelle partie ».
	Page = EFSPage::Title;
	UpdatePause();
	ApplyVolumes();
}

UFSMissionSubsystem* AFSHUD::Mission() const
{
	const UGameInstance* GI = GetGameInstance();
	return GI ? GI->GetSubsystem<UFSMissionSubsystem>() : nullptr;
}

AFSPrologueDirector* AFSHUD::Director()
{
	if (!CachedDirector.IsValid())
	{
		CachedDirector = Cast<AFSPrologueDirector>(UGameplayStatics::GetActorOfClass(this, AFSPrologueDirector::StaticClass()));
	}
	return CachedDirector.Get();
}

FString AFSHUD::UIText(const TCHAR* Id, const TCHAR* Fallback) const
{
	const UFSMissionSubsystem* M = Mission();
	return M ? M->GetUIText(FName(Id), Fallback) : FString(Fallback);
}

float AFSHUD::Now() const
{
	// Temps de jeu : s'arrête pendant la pause (écran titre), comme l'horloge du prologue.
	return GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
}

float AFSHUD::Ui() const
{
	return Canvas ? FMath::Max(0.6f, Canvas->ClipY / 1080.f) : 1.f;
}

UFSPhoneSubsystem* AFSHUD::Phone() const
{
	const UGameInstance* GI = GetGameInstance();
	return GI ? GI->GetSubsystem<UFSPhoneSubsystem>() : nullptr;
}

AFSHeroCharacter* AFSHUD::Hero() const
{
	return Cast<AFSHeroCharacter>(GetOwningPawn());
}

FString AFSHUD::ClockText()
{
	AFSPrologueDirector* D = Director();
	const int32 Secs = D ? FMath::FloorToInt(D->GetPrologueSeconds()) : 0;
	return FString::Printf(TEXT("%02d:%02d"), 16 + (25 + Secs / 60) / 60, (25 + Secs / 60) % 60);
}

void AFSHUD::Flash()
{
	FlashStart = Now();
	PlayUi(TEXT("SFX_Photo"));
}

void AFSHUD::NotifyPhone()
{
	PlayUi(TEXT("SFX_Notification"));
	ShowToast(TEXT("Téléphone : nouveau message (O)"), 4.f);
}

void AFSHUD::HandleAcquired(FName Id)
{
	if (Id.ToString().StartsWith(TEXT("CLU_")))
	{
		ShowToast(UIText(TEXT("UI_CARNET_NOUVEAU"), TEXT("Noté dans le carnet (J)")), 4.f);
	}
}

void AFSHUD::ShowLine(const FString& Speaker, const FString& InText, float Seconds)
{
	if (InText.IsEmpty())
	{
		return;
	}
	// Le carton d'ouverture affiche déjà la première narration : pas de doublon en sous-titre.
	if (Speaker.IsEmpty() && InText == IntroText && Now() - IntroStart < IntroSeconds)
	{
		return;
	}
	Subtitle.Speaker = Speaker;
	Subtitle.Text = InText;
	Subtitle.Until = Now() + FMath::Max(Seconds, 2.f + 0.06f * InText.Len());
}

void AFSHUD::ShowHint(int32 Key, const FString& InText, float Seconds)
{
	FTimedText& H = Hints.FindOrAdd(Key);
	H.Text = InText;
	H.Until = Now() + Seconds;
}

void AFSHUD::ShowToast(const FString& InText, float Seconds)
{
	FTimedText& T = Toasts.AddDefaulted_GetRef();
	T.Text = InText;
	T.Until = Now() + Seconds;
	if (Toasts.Num() > 4)
	{
		Toasts.RemoveAt(0);
	}
}

// --- Dessin -------------------------------------------------------------------------------

void AFSHUD::Box(float X, float Y, float W, float H, const FLinearColor& Color)
{
	FCanvasTileItem Tile(FVector2D(X, Y), FVector2D(W, H), Color);
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}

FVector2D AFSHUD::Measure(const FString& S, UFont* Font, float Scale)
{
	if (!Font)
	{
		return FVector2D::ZeroVector;
	}
	return FVector2D(Font->GetStringSize(*S) * Scale, Font->GetMaxCharHeight() * Scale);
}

void AFSHUD::Text(const FString& S, float X, float Y, UFont* Font, float Scale, const FLinearColor& Color, bool bCentreX)
{
	if (bCentreX)
	{
		X -= 0.5f * Measure(S, Font, Scale).X;
	}
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(S), Font, Color);
	Item.Scale = FVector2D(Scale, Scale);
	Item.EnableShadow(FLinearColor(0.f, 0.f, 0.f, 0.85f));
	Canvas->DrawItem(Item);
}

TArray<FString> AFSHUD::Wrap(const FString& S, UFont* Font, float Scale, float MaxWidth)
{
	TArray<FString> Words;
	S.ParseIntoArray(Words, TEXT(" "));
	TArray<FString> Out;
	FString Line;
	for (const FString& Word : Words)
	{
		const FString Candidate = Line.IsEmpty() ? Word : Line + TEXT(" ") + Word;
		if (!Line.IsEmpty() && Measure(Candidate, Font, Scale).X > MaxWidth)
		{
			Out.Add(Line);
			Line = Word;
		}
		else
		{
			Line = Candidate;
		}
	}
	if (!Line.IsEmpty())
	{
		Out.Add(Line);
	}
	return Out;
}

void AFSHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas || !GEngine)
	{
		return;
	}
	if (!bBound)
	{
		if (UFSMissionSubsystem* M = Mission())
		{
			M->OnAcquired.AddDynamic(this, &AFSHUD::HandleAcquired);
			bBound = true;
		}
	}
	if (Page == EFSPage::Title)
	{
		DrawPage();
		return;
	}
	UpdateConversation();
	AFSPrologueDirector* D = Director();
	if (D && D->GetPhaseIndex() == PhaseChapter1)
	{
		DrawEndScreen();
		if (Page != EFSPage::None) { DrawPage(); }
		return;
	}
	DrawAlleyMarker();
	DrawClockAndObjective();
	DrawSubtitles();
	DrawHintsAndToasts();
	DrawPrompt();
	DrawIntroCard();
	DrawFlash();
	if (bNotebook)
	{
		DrawNotebook();
	}
	if (Page != EFSPage::None)
	{
		DrawPage();
	}
}

void AFSHUD::DrawIntroCard()
{
	const float Age = Now() - IntroStart;
	if (IntroStart < 0.f || Age > IntroSeconds || IntroText.IsEmpty())
	{
		return;
	}
	// Fondu : 1 s d'apparition, 1,5 s de disparition.
	const float Alpha = FMath::Clamp(FMath::Min(Age / 1.f, (IntroSeconds - Age) / 1.5f), 0.f, 1.f);
	const float U = Ui();
	Box(0.f, Canvas->ClipY * 0.40f, Canvas->ClipX, 130.f * U, FLinearColor(0.f, 0.f, 0.f, 0.6f * Alpha));
	Text(IntroText, 0.5f * Canvas->ClipX, Canvas->ClipY * 0.40f + 38.f * U, GEngine->GetLargeFont(), 1.6f * U,
		FLinearColor(Cream.R, Cream.G, Cream.B, Alpha), true);
}

void AFSHUD::DrawClockAndObjective()
{
	AFSPrologueDirector* D = Director();
	if (!D)
	{
		return;
	}
	const float U = Ui();
	UFont* Medium = GEngine->GetMediumFont();
	UFont* Small = GEngine->GetSmallFont();

	// Horloge en haut à droite : mardi 16:25:00 + secondes écoulées du prologue.
	const int32 Secs = FMath::FloorToInt(D->GetPrologueSeconds());
	const FString Clock = FString::Printf(TEXT("Mardi  %02d:%02d"), 16 + (25 + Secs / 60) / 60, (25 + Secs / 60) % 60);
	const FVector2D ClockSize = Measure(Clock, Medium, 1.3f * U);
	Box(Canvas->ClipX - ClockSize.X - 44.f * U, 24.f * U, ClockSize.X + 24.f * U, ClockSize.Y + 14.f * U, Shade);
	Text(Clock, Canvas->ClipX - ClockSize.X - 32.f * U, 31.f * U, Medium, 1.3f * U, Cream);

	// Objectif en haut à gauche, selon la phase du prologue.
	const int32 Phase = D->GetPhaseIndex();
	const float T = D->GetPrologueSeconds();
	FString Objective;
	FString Detail;
	switch (Phase)
	{
	case PhaseBefore:
		Objective = T < 106.f ? UIText(TEXT("UI_OBJ_P_BALADE"), TEXT("Promène-toi avec Ariane sur la place de l'école."))
			: T < 240.f ? UIText(TEXT("UI_OBJ_P_SORTIE"), TEXT("Les enfants sortent de l'école."))
			: UIText(TEXT("UI_OBJ_P_RUELLE"), TEXT("Ariane fixe la ruelle. Va voir."));
		break;
	case PhaseWindow:
		Objective = UIText(TEXT("UI_OBJ_P_AGIR"), TEXT("Agis, vite ! Deux actions au plus."));
		Detail = FString::Printf(TEXT("%d s · %d action%s possible%s"), FMath::CeilToInt(D->GetWindowRemaining()),
			D->GetActionsLeft(), D->GetActionsLeft() > 1 ? TEXT("s") : TEXT(""), D->GetActionsLeft() > 1 ? TEXT("s") : TEXT(""));
		break;
	case PhaseHolding:
		Objective = UIText(TEXT("UI_OBJ_P_REJOINDRE"), TEXT("Rejoins l'entrée de la ruelle !"));
		break;
	case PhaseDeparting:
		Objective = UIText(TEXT("UI_OBJ_P_FUITE"), TEXT("Le fourgon s'enfuit."));
		break;
	default:
		Objective = UIText(TEXT("UI_OBJ_P_APPEL"), TEXT("Appelle le 17."));
		Detail = UIText(TEXT("UI_TOUCHES_APPEL"), TEXT("T : appeler le 17"));
		break;
	}
	const float MaxW = 560.f * U;
	const TArray<FString> Lines = Wrap(Objective, Medium, 1.05f * U, MaxW);
	const float LineH = Measure(TEXT("Ag"), Medium, 1.05f * U).Y;
	const float H = 44.f * U + LineH * Lines.Num() + (Detail.IsEmpty() ? 0.f : LineH);
	Box(24.f * U, 24.f * U, MaxW + 32.f * U, H, Shade);
	Box(24.f * U, 24.f * U, 5.f * U, H, Amber);
	Text(UIText(TEXT("UI_OBJECTIF"), TEXT("Objectif")).ToUpper(), 44.f * U, 32.f * U, Small, 1.0f * U, Amber);
	float Y = 32.f * U + 26.f * U;
	for (const FString& L : Lines)
	{
		Text(L, 44.f * U, Y, Medium, 1.05f * U, Cream);
		Y += LineH;
	}
	if (!Detail.IsEmpty())
	{
		Text(Detail, 44.f * U, Y, Medium, 1.0f * U, Amber);
	}
	if (Phase == PhaseWindow && D->GetWindowSeconds() > 0.f)
	{
		// Barre du temps restant sous le cadre d'objectif.
		const float Ratio = FMath::Clamp(D->GetWindowRemaining() / D->GetWindowSeconds(), 0.f, 1.f);
		Box(24.f * U, 24.f * U + H + 6.f * U, MaxW + 32.f * U, 8.f * U, Shade);
		Box(24.f * U, 24.f * U + H + 6.f * U, (MaxW + 32.f * U) * Ratio, 8.f * U, FLinearColor(1.f, 0.35f + 0.4f * Ratio, 0.2f, 0.95f));
	}
}

void AFSHUD::DrawSubtitles()
{
	if (Subtitle.Text.IsEmpty() || Now() > Subtitle.Until)
	{
		return;
	}
	const float U = Ui();
	UFont* Font = GEngine->GetMediumFont();
	const float Scale = 1.25f * U * FSSettings::SubtitleScale();
	const float MaxW = FMath::Min(1100.f * U, Canvas->ClipX - 80.f * U);
	const FString Full = Subtitle.Speaker.IsEmpty() ? Subtitle.Text : Subtitle.Speaker + TEXT(" : ") + Subtitle.Text;
	const TArray<FString> Lines = Wrap(Full, Font, Scale, MaxW);
	const float LineH = Measure(TEXT("Ag"), Font, Scale).Y;
	float Widest = 0.f;
	for (const FString& L : Lines) { Widest = FMath::Max(Widest, Measure(L, Font, Scale).X); }
	const float H = LineH * Lines.Num() + 20.f * U;
	const float Y0 = Canvas->ClipY - 120.f * U - H;
	const float CX = 0.5f * Canvas->ClipX;
	Box(CX - 0.5f * Widest - 20.f * U, Y0, Widest + 40.f * U, H, Shade);
	float Y = Y0 + 10.f * U;
	// Narration (carnet) en ambre, répliques en crème.
	const FLinearColor Color = Subtitle.Speaker.IsEmpty() ? Amber : Cream;
	for (const FString& L : Lines)
	{
		Text(L, CX, Y, Font, Scale, Color, true);
		Y += LineH;
	}
}

void AFSHUD::DrawHintsAndToasts()
{
	const float U = Ui();
	UFont* Small = GEngine->GetSmallFont();
	const float T = Now();

	// Rappels de commandes (fenêtre d'action, appel au 17) juste au-dessus des sous-titres.
	float Y = Canvas->ClipY - 100.f * U;
	for (const TPair<int32, FTimedText>& Pair : Hints)
	{
		if (Pair.Key == 2 || T > Pair.Value.Until || Pair.Value.Text.IsEmpty())
		{
			continue; // clé 2 : commandes de base, déjà affichées en permanence en bas.
		}
		Text(Pair.Value.Text, 0.5f * Canvas->ClipX, Y, GEngine->GetMediumFont(), 1.05f * U, Amber, true);
		Y -= 30.f * U;
	}
	// Commandes de base en permanence, discrètes.
	Text(UIText(TEXT("UI_TOUCHES_BASE"), TEXT("")), 0.5f * Canvas->ClipX, Canvas->ClipY - 40.f * U, Small, 1.0f * U, Soft, true);

	// Notifications en haut à droite, sous l'horloge.
	float TY = 84.f * U;
	for (int32 i = Toasts.Num() - 1; i >= 0; --i)
	{
		if (T > Toasts[i].Until)
		{
			Toasts.RemoveAt(i);
		}
	}
	for (const FTimedText& Toast : Toasts)
	{
		const FVector2D S = Measure(Toast.Text, Small, 1.1f * U);
		Box(Canvas->ClipX - S.X - 44.f * U, TY, S.X + 24.f * U, S.Y + 10.f * U, Shade);
		Text(Toast.Text, Canvas->ClipX - S.X - 32.f * U, TY + 5.f * U, Small, 1.1f * U, Cream);
		TY += S.Y + 16.f * U;
	}
}

void AFSHUD::DrawNotebook()
{
	const float U = Ui();
	UFont* Medium = GEngine->GetMediumFont();
	UFont* Large = GEngine->GetLargeFont();
	const float W = FMath::Min(900.f * U, Canvas->ClipX - 80.f * U);
	const float H = FMath::Min(640.f * U, Canvas->ClipY - 160.f * U);
	const float X0 = 0.5f * (Canvas->ClipX - W);
	const float Y0 = 0.5f * (Canvas->ClipY - H);
	Box(X0, Y0, W, H, FLinearColor(0.10f, 0.08f, 0.06f, 0.93f));
	Box(X0, Y0, W, 6.f * U, Amber);
	Text(UIText(TEXT("UI_CARNET_TITRE"), TEXT("Carnet d'enquête")), X0 + 32.f * U, Y0 + 24.f * U, Large, 1.3f * U, Cream);
	Text(UIText(TEXT("UI_CARNET_FAITS"), TEXT("Ce que j'ai constaté")), X0 + 32.f * U, Y0 + 80.f * U, Medium, 1.05f * U, Amber);
	const UFSMissionSubsystem* M = Mission();
	const TArray<FName> Clues = M ? M->GetAcquiredClues() : TArray<FName>();
	float Y = Y0 + 120.f * U;
	const float LineH = Measure(TEXT("Ag"), Medium, 1.0f * U).Y;
	if (Clues.Num() == 0)
	{
		Text(UIText(TEXT("UI_CARNET_VIDE"), TEXT("Rien de noté pour l'instant.")), X0 + 32.f * U, Y, Medium, 1.0f * U, Soft);
	}
	for (const FName& Id : Clues)
	{
		for (const FString& L : Wrap(TEXT("• ") + M->GetClueFact(Id), Medium, 1.0f * U, W - 64.f * U))
		{
			if (Y > Y0 + H - 50.f * U) { break; }
			Text(L, X0 + 32.f * U, Y, Medium, 1.0f * U, Cream);
			Y += LineH;
		}
		Y += 8.f * U;
	}
	Text(TEXT("J : fermer"), X0 + W - 32.f * U - Measure(TEXT("J : fermer"), GEngine->GetSmallFont(), 1.0f * U).X,
		Y0 + H - 34.f * U, GEngine->GetSmallFont(), 1.0f * U, Soft);
}

void AFSHUD::DrawEndScreen()
{
	const float U = Ui();
	const float CX = 0.5f * Canvas->ClipX;
	Box(0.f, 0.f, Canvas->ClipX, Canvas->ClipY, FLinearColor(0.02f, 0.02f, 0.03f, 0.88f));
	UFont* Large = GEngine->GetLargeFont();
	UFont* Medium = GEngine->GetMediumFont();
	float Y = Canvas->ClipY * 0.18f;
	Text(UIText(TEXT("UI_FIN_PROLOGUE_TITRE"), TEXT("Fin du prologue")), CX, Y, Large, 2.2f * U, Cream, true);
	Y += 90.f * U;
	const float MaxW = FMath::Min(1000.f * U, Canvas->ClipX - 80.f * U);
	const float LineH = Measure(TEXT("Ag"), Medium, 1.1f * U).Y;
	for (const FString& L : Wrap(UIText(TEXT("UI_FIN_PROLOGUE_TEXTE"), TEXT("16 h 31. Appel au 17.")), Medium, 1.1f * U, MaxW))
	{
		Text(L, CX, Y, Medium, 1.1f * U, Amber, true);
		Y += LineH;
	}
	Y += 30.f * U;
	Text(UIText(TEXT("UI_CARNET_FAITS"), TEXT("Ce que j'ai constaté")), CX, Y, Medium, 1.1f * U, Cream, true);
	Y += LineH + 10.f * U;
	const UFSMissionSubsystem* M = Mission();
	const TArray<FName> Clues = M ? M->GetAcquiredClues() : TArray<FName>();
	if (Clues.Num() == 0)
	{
		Text(UIText(TEXT("UI_CARNET_VIDE"), TEXT("Rien de noté pour l'instant.")), CX, Y, Medium, 1.0f * U, Soft, true);
		Y += LineH;
	}
	for (const FName& Id : Clues)
	{
		for (const FString& L : Wrap(TEXT("• ") + M->GetClueFact(Id), Medium, 1.0f * U, MaxW))
		{
			Text(L, CX, Y, Medium, 1.0f * U, Soft, true);
			Y += LineH;
		}
	}
	Text(TEXT("P : menu principal (recommencer, charger, quitter)"), CX, Canvas->ClipY - 70.f * U, GEngine->GetSmallFont(), 1.1f * U, Soft, true);
}

// --- Conversations, invites, repère, photo --------------------------------------------------

void AFSHUD::PlayConversation(const TArray<FName>& LineIds)
{
	Conversation = LineIds;
	ConversationNext = Now();
}

void AFSHUD::UpdateConversation()
{
	if (Conversation.Num() == 0 || Now() < ConversationNext)
	{
		return;
	}
	const FName Id = Conversation[0];
	Conversation.RemoveAt(0);
	if (const UFSMissionSubsystem* M = Mission())
	{
		const FString Line = M->GetLineText(Id);
		ShowLine(M->GetLineSpeaker(Id), Line, 0.f);
		ConversationNext = Subtitle.Until + 0.3f;
	}
}

void AFSHUD::DrawPrompt()
{
	const AFSHeroCharacter* Hero = Cast<AFSHeroCharacter>(GetOwningPawn());
	const FString Prompt = Hero ? Hero->GetInteractionPrompt() : FString();
	if (Prompt.IsEmpty() || Page != EFSPage::None)
	{
		return;
	}
	const float U = Ui();
	UFont* Medium = GEngine->GetMediumFont();
	const FVector2D S = Measure(Prompt, Medium, 1.1f * U);
	const float X = 0.5f * (Canvas->ClipX - S.X) - 16.f * U;
	const float Y = Canvas->ClipY * 0.62f;
	Box(X, Y, S.X + 32.f * U, S.Y + 14.f * U, Shade);
	Box(X, Y + S.Y + 10.f * U, S.X + 32.f * U, 4.f * U, Amber);
	Text(Prompt, X + 16.f * U, Y + 7.f * U, Medium, 1.1f * U, Cream);
}

void AFSHUD::DrawAlleyMarker()
{
	AFSPrologueDirector* D = Director();
	APlayerController* PC = GetOwningPlayerController();
	if (!D || !PC || !D->GetEntrance())
	{
		return;
	}
	const int32 Phase = D->GetPhaseIndex();
	const bool bShow = Phase == PhaseHolding || Phase == PhaseWindow || (Phase == PhaseBefore && D->GetPrologueSeconds() >= 240.f);
	const APawn* Pawn = PC->GetPawn();
	if (!bShow || !Pawn)
	{
		return;
	}
	const FVector Target = D->GetEntrance()->GetActorLocation() + FVector(0.f, 0.f, 180.f);
	const float Metres = FVector::Dist2D(Pawn->GetActorLocation(), Target) / 100.f;
	if (Metres < 4.f)
	{
		return;
	}
	const float U = Ui();
	const float Margin = 60.f * U;
	FVector2D Screen;
	const bool bInFront = UGameplayStatics::ProjectWorldToScreen(PC, Target, Screen);
	int32 VX = 0, VY = 0;
	PC->GetViewportSize(VX, VY);
	if (VX > 0 && VY > 0)
	{
		// Coordonnées de la fenêtre -> coordonnées du canevas (identiques sauf mise à l'échelle DPI).
		Screen.X *= Canvas->ClipX / VX;
		Screen.Y *= Canvas->ClipY / VY;
	}
	if (!bInFront)
	{
		// Derrière la caméra : repère ramené au bord bas, du côté où il faut tourner.
		const FVector ToTarget = (Target - PC->PlayerCameraManager->GetCameraLocation()).GetSafeNormal2D();
		const FVector Right = PC->PlayerCameraManager->GetCameraRotation().RotateVector(FVector::RightVector).GetSafeNormal2D();
		Screen = FVector2D(FVector::DotProduct(ToTarget, Right) >= 0.f ? Canvas->ClipX - Margin : Margin, Canvas->ClipY - 3.f * Margin);
	}
	Screen.X = FMath::Clamp(Screen.X, Margin, Canvas->ClipX - Margin);
	Screen.Y = FMath::Clamp(Screen.Y, 2.f * Margin, Canvas->ClipY - 3.f * Margin);
	const FString Label = FString::Printf(TEXT("%s  %d m"), *UIText(TEXT("UI_REPERE_RUELLE"), TEXT("Ruelle")), FMath::RoundToInt(Metres));
	UFont* Small = GEngine->GetSmallFont();
	const FVector2D S = Measure(Label, Small, 1.1f * U);
	const float Pulse = 0.7f + 0.3f * FMath::Abs(FMath::Sin(GetWorld()->GetRealTimeSeconds() * 3.f));
	Box(Screen.X - 7.f * U, Screen.Y - 7.f * U, 14.f * U, 14.f * U, FLinearColor(Amber.R, Amber.G, Amber.B, Pulse));
	Box(Screen.X - 0.5f * S.X - 10.f * U, Screen.Y + 12.f * U, S.X + 20.f * U, S.Y + 8.f * U, Shade);
	Text(Label, Screen.X, Screen.Y + 16.f * U, Small, 1.1f * U, Cream, true);
}

void AFSHUD::DrawFlash()
{
	const float Age = Now() - FlashStart;
	if (Age < 0.f || Age > 0.35f)
	{
		return;
	}
	Box(0.f, 0.f, Canvas->ClipX, Canvas->ClipY, FLinearColor(1.f, 1.f, 1.f, 0.85f * (1.f - Age / 0.35f)));
}
