#include "FSPrologueDirector.h"
#include "FauxSemblants.h"
#include "FSDogCharacter.h"
#include "FSHUD.h"
#include "FSHeroCharacter.h"
#include "FSMissionSubsystem.h"
#include "FSSettings.h"
#include "FSPhoneSubsystem.h"
#include "FSSaveGame.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	// Chronologie du découpage (PR #4), secondes depuis 16:25:00, points de la maquette de Codex (PR #3).
	const FFSKey LilaKeys[] = {
		{0, 1.f, 1.6f}, {120, 1.f, 1.6f}, {128, 1.f, -0.5f}, {141, 10.f, -6.f}, {151, 18.5f, -12.f},
		{161, 33.f, -15.6f}, {176, 45.6f, -16.2f}, {215, 45.6f, -16.2f}, {240, 62.2f, -16.2f}, {400, 62.2f, -16.2f} };
	const FFSKey K2Keys[] = {
		{0, 47.4f, -16.1f}, {176, 47.4f, -16.1f}, {215, 46.6f, -16.f}, {240, 63.2f, -16.f}, {400, 63.2f, -16.f} };

	struct FCue { float T; const TCHAR* Line; const TCHAR* Caption; };
	const FCue Cues[] = {
		{0, TEXT("DLG_P_TUTO_01"), nullptr},
		{12, TEXT("DLG_P_TUTO_02"), nullptr},
		{24, TEXT("DLG_P_TUTO_03"), nullptr},
		{106, nullptr, TEXT("16 h 26 : la sonnerie. Les enfants sortent.")},
		{120, TEXT("DLG_P_LILA_01"), nullptr},
		{176, TEXT("DLG_P_ABORDAGE_01"), nullptr},
		{190, TEXT("DLG_P_ABORDAGE_02"), nullptr},
		{205, TEXT("DLG_P_ABORDAGE_03"), nullptr},
		{242, TEXT("DLG_P_ABORDAGE_04"), nullptr},
		{255, TEXT("DLG_P_ABORDAGE_05"), nullptr},
		{272, TEXT("DLG_P_ABORDAGE_06"), nullptr} };

	// Événements du prologue qui délivrent des éléments du téléphone (GameData/telephone/01-prologue.json).
	struct FPhoneCue { float T; const TCHAR* Event; };
	const FPhoneCue PhoneCues[] = { {106, TEXT("EVT_SONNERIE")}, {120, TEXT("EVT_LILA_COUCOU")}, {176, TEXT("EVT_ABORDAGE")} };

	float JsonNumber(const TSharedPtr<FJsonObject>& Obj, const TCHAR* Field, float Default)
	{
		double V = Default;
		return (Obj.IsValid() && Obj->TryGetNumberField(Field, V)) ? static_cast<float>(V) : Default;
	}
}

AFSPrologueDirector::AFSPrologueDirector()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

AActor* AFSPrologueDirector::FindTagged(FName Tag) const
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(Tag))
		{
			return *It;
		}
	}
	UE_LOG(LogFauxSemblants, Warning, TEXT("Acteur au tag %s introuvable (relancer Scripts/setup_prologue.py)"), *Tag.ToString());
	return nullptr;
}

void AFSPrologueDirector::BeginPlay()
{
	Super::BeginPlay();
	LilaPath = TArray<FFSKey>(LilaKeys, UE_ARRAY_COUNT(LilaKeys));
	K2Path = TArray<FFSKey>(K2Keys, UE_ARRAY_COUNT(K2Keys));
	Lila = FindTagged(TEXT("Lila"));
	K2 = FindTagged(TEXT("K2"));
	Van = FindTagged(TEXT("Fourgon"));
	Entrance = FindTagged(TEXT("EntreeRuelle"));
	Keyring = FindTagged(TEXT("PorteCles"));
	Bracelet = FindTagged(TEXT("Bracelet"));
	DepartCourtCamera = FindTagged(TEXT("CAM_DEPART_COURT"));
	if (Van) { VanStart = Van->GetActorLocation(); }
	if (Keyring) { Keyring->SetActorHiddenInGame(true); }
	if (Bracelet) { Bracelet->SetActorHiddenInGame(true); }

	Mission = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>();
	if (Mission && Mission->LoadMission(MissionFolder))
	{
		const TSharedPtr<FJsonObject> Pro = Mission->GetPrologue();
		const TArray<TSharedPtr<FJsonValue>>* Window = nullptr;
		if (Pro.IsValid() && Pro->TryGetArrayField(TEXT("window_seconds"), Window) && Window->Num() == 2)
		{
			WindowSeconds = 0.5f * static_cast<float>((*Window)[0]->AsNumber() + (*Window)[1]->AsNumber());
		}
		MaxActions = static_cast<int32>(JsonNumber(Pro, TEXT("window_max_actions"), 2.f));
		const TSharedPtr<FJsonObject>* Rule = nullptr;
		if (Pro.IsValid() && Pro->TryGetObjectField(TEXT("departure_rule"), Rule))
		{
			FString Latest;
			if ((*Rule)->TryGetStringField(TEXT("alert_latest"), Latest))
			{
				AlertLatest = static_cast<float>(UFSMissionSubsystem::ParseClockSeconds(Latest) - UFSMissionSubsystem::ParseClockSeconds(TEXT("16:25:00")));
			}
			AlertDistanceCm = 100.f * JsonNumber(*Rule, TEXT("alert_trigger_distance_m"), 5.f);
			HoldDistanceCm = 100.f * JsonNumber(*Rule, TEXT("hold_until_player_within_m_of_entrance"), 5.f);
			HoldMaxS = JsonNumber(*Rule, TEXT("hold_max_s"), 30.f);
			DescentS = JsonNumber(*Rule, TEXT("descent_to_turn_s"), 12.f);
			const TSharedPtr<FJsonObject>* Shot = nullptr;
			if ((*Rule)->TryGetObjectField(TEXT("fallback_shot"), Shot))
			{
				ShotS = JsonNumber(*Shot, TEXT("duration_s"), 4.f);
			}
		}
	}
	if (UFSPhoneSubsystem* Phone = GetGameInstance()->GetSubsystem<UFSPhoneSubsystem>())
	{
		Phone->LoadPhone(MissionFolder, Mission ? Mission->HeroineName : FString(TEXT("Audrey")));
	}
	Say(TEXT("Déplacement : ZQSD ou WASD · Maj : courir · 1/2/3 ou V : vues · Tab : épaule · R : rappeler Ariane"), 12.f, 2);
}

void AFSPrologueDirector::Say(const FString& Text, float Seconds, int32 Key)
{
	if (Text.IsEmpty())
	{
		return;
	}
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		// Clés 2, 3, 7 : rappels de commandes ; 20 et plus : indices notés (le HUD les signale déjà) ; sinon narration.
		if (Key == 2 || Key == 3 || Key == 7) { Hud->ShowHint(Key, Text, Seconds); }
		else if (Key < 20) { Hud->ShowLine(FString(), Text, Seconds); }
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Seconds, FColor(255, 236, 200), Text);
	}
}

void AFSPrologueDirector::SayLine(const TCHAR* LineId, float Seconds)
{
	if (!Mission)
	{
		return;
	}
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		Hud->ShowLine(Mission->GetLineSpeaker(FName(LineId)), Mission->GetLineText(FName(LineId)), Seconds);
		return;
	}
	Say(Mission->GetLineText(FName(LineId)), Seconds);
}

void AFSPrologueDirector::MoveAlong(AActor* Actor, const TArray<FFSKey>& Keys, float Time, float Height) const
{
	if (!Actor || Keys.Num() == 0)
	{
		return;
	}
	FFSKey A = Keys[0], B = Keys[0];
	for (int32 i = 1; i < Keys.Num(); ++i)
	{
		B = Keys[i];
		if (Time <= B.T) { break; }
		A = B;
	}
	const float U = (B.T > A.T) ? FMath::Clamp((Time - A.T) / (B.T - A.T), 0.f, 1.f) : 1.f;
	const FVector P = FMath::Lerp(FromBlockout(A.X, A.Z, Height), FromBlockout(B.X, B.Z, Height), U);
	const FVector Dir = FromBlockout(B.X, B.Z) - FromBlockout(A.X, A.Z);
	Actor->SetActorLocation(P);
	if (!Dir.IsNearlyZero() && U < 1.f)
	{
		Actor->SetActorRotation(Dir.Rotation());
	}
}

float AFSPrologueDirector::DistanceToEntrance(bool bIncludeDog) const
{
	const APawn* Hero = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Hero || !Entrance)
	{
		return TNumericLimits<float>::Max();
	}
	float D = FVector::Dist2D(Hero->GetActorLocation(), Entrance->GetActorLocation());
	for (TActorIterator<AFSDogCharacter> It(GetWorld()); It; ++It)
	{
		if (bIncludeDog)
		{
			D = FMath::Min(D, FVector::Dist2D(It->GetActorLocation(), Entrance->GetActorLocation()));
		}
	}
	return D;
}

void AFSPrologueDirector::Grant(const TArray<FName>& Ids)
{
	for (const FName& Id : Ids)
	{
		if (Mission)
		{
			Mission->Grant(Id);
			Say(FString::Printf(TEXT("Carnet : %s"), *Mission->GetClueFact(Id)), 8.f, 20 + ActionsTaken.Num());
		}
	}
}

bool AFSPrologueDirector::CanWave() const
{
	const APawn* Hero = UGameplayStatics::GetPlayerPawn(this, 0);
	return !bWaved && Phase == EPhase::Before && T >= 120.f && T <= 150.f && Lila && Hero
		&& FVector::Dist2D(Hero->GetActorLocation(), Lila->GetActorLocation()) < 3500.f;
}

bool AFSPrologueDirector::TryWave()
{
	if (!CanWave())
	{
		return false;
	}
	bWaved = true;
	for (TActorIterator<AFSDogCharacter> It(GetWorld()); It; ++It) { It->SetMood(EFSDogMood::Happy, 4.f); }
	if (Mission)
	{
		Mission->Grant(TEXT("FLAG_COUCOU_RENDU"));
		if (AFSHUD* Hud = AFSHUD::Get(this))
		{
			Hud->PlayConversation({ FName(TEXT("DLG_P_LILA_02")), FName(TEXT("DLG_P_LILA_03")) });
		}
	}
	return true;
}

void AFSPrologueDirector::OpenWindow()
{
	Phase = EPhase::Window;
	AlertT = T;
	// Option d'accessibilité : fenêtre allongée (window_accessibility), même nombre d'actions.
	if (FSSettings::ExtendedActionTime() && Mission)
	{
		const TSharedPtr<FJsonObject> Pro = Mission->GetPrologue();
		const TSharedPtr<FJsonObject>* Access = nullptr;
		const TArray<TSharedPtr<FJsonValue>>* Range = nullptr;
		if (Pro.IsValid() && Pro->TryGetObjectField(TEXT("window_accessibility"), Access)
			&& (*Access)->TryGetArrayField(TEXT("window_seconds"), Range) && Range->Num() == 2)
		{
			WindowSeconds = 0.5f * static_cast<float>((*Range)[0]->AsNumber() + (*Range)[1]->AsNumber());
		}
	}
	PhoneEvent(TEXT("EVT_ALERTE"));
	SayLine(TEXT("DLG_P_ALERTE_01"), 5.f);
	Say(TEXT("F : photographier · Maj : courir · C : crier « Lila ! » · E : « Ariane, va ! » (deux actions au plus)"), WindowSeconds, 3);
}

void AFSPrologueDirector::TryAction(FName ActionId)
{
	if (Phase != EPhase::Window && Phase != EPhase::Holding)
	{
		return;
	}
	if (ActionsTaken.Contains(ActionId) || ActionsTaken.Num() >= MaxActions || !Mission)
	{
		return;
	}
	const TSharedPtr<FJsonObject> Pro = Mission->GetPrologue();
	const TArray<TSharedPtr<FJsonValue>>* Actions = nullptr;
	if (!Pro.IsValid() || !Pro->TryGetArrayField(TEXT("actions"), Actions))
	{
		return;
	}
	ActionsTaken.Add(ActionId);
	for (const TSharedPtr<FJsonValue>& V : *Actions)
	{
		const TSharedPtr<FJsonObject> A = V->AsObject();
		if (A->GetStringField(TEXT("id")) != ActionId.ToString())
		{
			continue;
		}
		// Photo prise en courant = floue au lieu de nette (grants_instead_if_with), jamais les deux.
		const AFSHeroCharacter* Hero = Cast<AFSHeroCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		const bool bRunning = Hero && Hero->IsRunning();
		const TSharedPtr<FJsonObject>* Instead = nullptr;
		const TArray<TSharedPtr<FJsonValue>>* GrantsJson = nullptr;
		const bool bReplaced = bRunning && ActionId != FName(TEXT("ACT_COURIR"))
			&& A->TryGetObjectField(TEXT("grants_instead_if_with"), Instead)
			&& (*Instead)->TryGetArrayField(TEXT("ACT_COURIR"), GrantsJson);
		if (!bReplaced)
		{
			A->TryGetArrayField(TEXT("grants"), GrantsJson);
		}
		TArray<FName> Ids;
		if (GrantsJson)
		{
			for (const TSharedPtr<FJsonValue>& G : *GrantsJson) { Ids.Add(FName(*G->AsString())); }
		}
		Grant(Ids);
	}
	if (ActionId == FName(TEXT("ACT_ENVOYER")) && Van)
	{
		for (TActorIterator<AFSDogCharacter> It(GetWorld()); It; ++It)
		{
			// Ariane s'arrête toujours au bord de la chaussée, jamais devant le fourgon.
			It->SendTo(FromBlockout(58.5f, -16.6f));
		}
	}
	if (ActionId == FName(TEXT("ACT_CRIER")))
	{
		SayLine(TEXT("DLG_P_ALERTE_05"), 4.f);
	}
}

void AFSPrologueDirector::StartDeparture()
{
	Phase = EPhase::Departing;
	DepartT = T;
	PhoneEvent(TEXT("EVT_DEPART"));
	if (Lila) { Lila->SetActorHiddenInGame(true); Lila->SetActorEnableCollision(false); }
	if (K2) { K2->SetActorHiddenInGame(true); K2->SetActorEnableCollision(false); }
	if (Bracelet) { Bracelet->SetActorHiddenInGame(false); }
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (bFallbackShot && PC && DepartCourtCamera)
	{
		PC->SetViewTargetWithBlend(DepartCourtCamera, 0.4f);
		Say(TEXT("(plan court : le départ du fourgon vu depuis l'entrée de la ruelle)"), ShotS, 4);
	}
}

void AFSPrologueDirector::CallPolice()
{
	if (Phase != EPhase::Gone)
	{
		return;
	}
	Phase = EPhase::Chapter1;
	PhoneEvent(TEXT("EVT_CH1_START"));
	if (Mission)
	{
		Mission->Grant(TEXT("FLAG_APPEL_17"));
		Mission->SetClockMinutes(16 * 60 + 31);
		Mission->SaveProgress();
	}
	Say(TEXT("16 h 31 — Appel au 17. Fin du prologue jouable (le chapitre 1 viendra au jalon suivant)."), 15.f, 5);
}

void AFSPrologueDirector::PhoneEvent(const TCHAR* EventId)
{
	UFSPhoneSubsystem* Phone = GetGameInstance()->GetSubsystem<UFSPhoneSubsystem>();
	if (Phone && Phone->Trigger(EventId) > 0)
	{
		if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->NotifyPhone(); }
	}
}

void AFSPrologueDirector::ApplyPhaseVisibility()
{
	const bool bGoneFigures = Phase >= EPhase::Departing;
	if (Lila) { Lila->SetActorHiddenInGame(bGoneFigures || T < 120.f); Lila->SetActorEnableCollision(!bGoneFigures && T >= 120.f); }
	if (K2) { K2->SetActorHiddenInGame(bGoneFigures); K2->SetActorEnableCollision(!bGoneFigures); }
	if (Bracelet) { Bracelet->SetActorHiddenInGame(!bGoneFigures); }
	if (Keyring) { Keyring->SetActorHiddenInGame(T < 176.f); }
	if (Van)
	{
		const bool bVanGone = Phase >= EPhase::Gone;
		Van->SetActorHiddenInGame(bVanGone);
		Van->SetActorEnableCollision(!bVanGone);
		if (Phase < EPhase::Departing) { Van->SetActorLocation(VanStart); }
	}
}

void AFSPrologueDirector::WriteState(UFSSaveGame& Save) const
{
	Save.bHasPrologue = true;
	Save.PrologueSeconds = T;
	Save.Phase = static_cast<uint8>(Phase);
	Save.AlertT = AlertT;
	Save.DepartT = DepartT;
	Save.WindowSeconds = WindowSeconds;
	Save.ActionsTaken = ActionsTaken;
	Save.LastCue = LastCue;
	Save.bWaved = bWaved;
}

void AFSPrologueDirector::ReadState(const UFSSaveGame& Save)
{
	if (!Save.bHasPrologue)
	{
		return;
	}
	T = Save.PrologueSeconds;
	Phase = static_cast<EPhase>(FMath::Min<uint8>(Save.Phase, static_cast<uint8>(EPhase::Chapter1)));
	AlertT = Save.AlertT;
	DepartT = Save.DepartT;
	WindowSeconds = Save.WindowSeconds;
	ActionsTaken = Save.ActionsTaken;
	LastCue = Save.LastCue;
	bWaved = Save.bWaved;
	bFallbackShot = false; // le plan court ne reprend pas : la vue revient à l'héroïne
	bDogReacted = T >= 232.f;
	LastPhoneEvent = -1;
	for (int32 i = 0; i < static_cast<int32>(UE_ARRAY_COUNT(PhoneCues)); ++i)
	{
		if (PhoneCues[i].T <= T) { LastPhoneEvent = i; }
	}
	ApplyPhaseVisibility();
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetViewTarget(PC->GetPawn());
	}
}

void AFSPrologueDirector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	T += DeltaSeconds * TimeScale;

	// Horloge affichée : 16:25:00 + T.
	const int32 Secs = FMath::FloorToInt(T);
	if (GEngine && Phase != EPhase::Chapter1 && !AFSHUD::Get(this))
	{
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::White, FString::Printf(TEXT("Mardi %02d:%02d:%02d"), 16, 25 + Secs / 60, Secs % 60));
	}

	for (int32 i = LastCue + 1; i < static_cast<int32>(UE_ARRAY_COUNT(Cues)); ++i)
	{
		if (T < Cues[i].T) { break; }
		LastCue = i;
		if (Cues[i].Line) { SayLine(Cues[i].Line); }
		if (Cues[i].Caption) { Say(Cues[i].Caption); }
	}

	for (int32 i = LastPhoneEvent + 1; i < static_cast<int32>(UE_ARRAY_COUNT(PhoneCues)); ++i)
	{
		if (T < PhoneCues[i].T) { break; }
		LastPhoneEvent = i;
		PhoneEvent(PhoneCues[i].Event);
	}

	if (Phase == EPhase::Before || Phase == EPhase::Window || Phase == EPhase::Holding)
	{
		if (Lila) { Lila->SetActorHiddenInGame(T < 120.f); Lila->SetActorEnableCollision(T >= 120.f); }
		MoveAlong(Lila, LilaPath, FMath::Min(T, 280.f), 0.65f);
		MoveAlong(K2, K2Path, FMath::Min(T, 280.f), 0.84f);
		if (Keyring && T >= 176.f) { Keyring->SetActorHiddenInGame(false); }
	}

	const float Dist = DistanceToEntrance();
	switch (Phase)
	{
	case EPhase::Before:
		// Ariane sent que quelque chose se passe dans la ruelle (comportements décrits par Audrey) :
		// de loin, elle vient « parler » et emmène l'héroïne vers l'angle ; de près, elle part comme une flèche.
		if (!bDogReacted && T >= 200.f && Entrance)
		{
			for (TActorIterator<AFSDogCharacter> It(GetWorld()); It; ++It)
			{
				It->SetPointOfInterest(Entrance->GetActorLocation());
				const float HeroDist = DistanceToEntrance(false);
				if (HeroDist > 2500.f)
				{
					It->LeadTo(Entrance->GetActorLocation());
					bDogReacted = true;
				}
				else if (T >= 232.f)
				{
					It->Bolt(Entrance->GetActorLocation());
					bDogReacted = true;
				}
			}
		}
		// Alerte : proximité de l'entrée une fois Lila devant la portière, ou heure limite (Ariane s'élance).
		if ((T >= 240.f && Dist <= AlertDistanceCm) || T >= AlertLatest)
		{
			if (Dist > AlertDistanceCm)
			{
				Say(TEXT("Ariane s'élance vers la ruelle en aboyant. Lila crie : « Ariane ! »"), 5.f, 6);
				for (TActorIterator<AFSDogCharacter> It(GetWorld()); It; ++It) { It->SendTo(Entrance ? Entrance->GetActorLocation() : FVector::ZeroVector); }
			}
			OpenWindow();
		}
		break;
	case EPhase::Window:
	{
		const AFSHeroCharacter* Hero = Cast<AFSHeroCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (Hero && Hero->IsRunning() && Hero->GetVelocity().Size2D() > 300.f) { TryAction(TEXT("ACT_COURIR")); }
		if (T - AlertT >= WindowSeconds)
		{
			Phase = EPhase::Holding;
		}
		break;
	}
	case EPhase::Holding:
		// Départ retenu (Lila résiste) tant que la joueuse n'est pas à l'entrée de la ruelle, hold_max_s au plus.
		if (DistanceToEntrance(false) <= HoldDistanceCm)
		{
			StartDeparture();
		}
		else if (T - AlertT - WindowSeconds >= HoldMaxS)
		{
			bFallbackShot = true;
			StartDeparture();
		}
		break;
	case EPhase::Departing:
	{
		const float U = FMath::Clamp((T - DepartT) / DescentS, 0.f, 1.f);
		if (Van)
		{
			// Le fourgon descend la ruelle vers le débouché (≈ 80 m) en accélérant.
			Van->SetActorLocation(VanStart + FVector(8000.f * U * U, 0.f, 0.f));
		}
		if (bFallbackShot && T - DepartT >= ShotS)
		{
			bFallbackShot = false;
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
			{
				PC->SetViewTargetWithBlend(PC->GetPawn(), 0.4f);
			}
		}
		if (U >= 1.f)
		{
			if (Van) { Van->SetActorHiddenInGame(true); Van->SetActorEnableCollision(false); }
			Phase = EPhase::Gone;
			PhoneEvent(TEXT("EVT_HORS_VUE"));
			for (TActorIterator<AFSDogCharacter> It(GetWorld()); It; ++It) { It->SetMood(EFSDogMood::Sad, 12.f); }
			SayLine(TEXT("DLG_P_HEROINE_CHOC_01"), 6.f);
			Say(TEXT("T : Appeler le 17"), 60.f, 7);
		}
		break;
	}
	default:
		break;
	}
}
