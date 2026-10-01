#include "FSHeroCharacter.h"
#include "FSPrologueDirector.h"
#include "FSDogCharacter.h"
#include "FSHUD.h"
#include "FSMissionSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "FSSettings.h"
#include "FSPhoneSubsystem.h"
#include "FSSaveGame.h"
#include "Components/SpotLightComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AFSHeroCharacter::AFSHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(30.f, 83.f); // 1,66 m

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	// Démarrages et arrêts nets, sans glisser (marche naturelle plutôt que patinage).
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1800.f;
	GetCharacterMovement()->GroundFriction = 9.f;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bDoCollisionTest = true; // la caméra se rapproche près des murs et des haies
	CameraArm->ProbeSize = 14.f;
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 12.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Lampe torche du sac, fixée à la caméra : éclaire là où l'on regarde, éteinte au départ.
	Torch = CreateDefaultSubobject<USpotLightComponent>(TEXT("Torch"));
	Torch->SetupAttachment(Camera);
	Torch->SetIntensity(6000.f);
	Torch->SetAttenuationRadius(2500.f);
	Torch->SetOuterConeAngle(24.f);
	Torch->SetInnerConeAngle(12.f);
	Torch->SetLightColor(FLinearColor(1.f, 0.95f, 0.85f));
	Torch->SetCastShadows(true);
	Torch->SetVisibility(false);

	PlaceholderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderBody"));
	PlaceholderBody->SetupAttachment(GetCapsuleComponent());
	PlaceholderBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (Cylinder.Succeeded())
	{
		PlaceholderBody->SetStaticMesh(Cylinder.Object);
		PlaceholderBody->SetRelativeScale3D(FVector(0.45f, 0.35f, 1.66f));
	}
}

void AFSHeroCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);
	Input->BindAxis(TEXT("MoveForward"), this, &AFSHeroCharacter::MoveForward);
	Input->BindAxis(TEXT("MoveRight"), this, &AFSHeroCharacter::MoveRight);
	Input->BindAxis(TEXT("Turn"), this, &AFSHeroCharacter::Turn);
	Input->BindAxis(TEXT("LookUp"), this, &AFSHeroCharacter::LookUp);
	Input->BindAxis(TEXT("TurnRate"), this, &AFSHeroCharacter::TurnRate);
	Input->BindAxis(TEXT("LookUpRate"), this, &AFSHeroCharacter::LookUpRate);
	Input->BindAxis(TEXT("Zoom"), this, &AFSHeroCharacter::Zoom);
	Input->BindAction(TEXT("Run"), IE_Pressed, this, &AFSHeroCharacter::StartRun);
	Input->BindAction(TEXT("Run"), IE_Released, this, &AFSHeroCharacter::StopRun);
	Input->BindAction(TEXT("CameraShoulder"), IE_Pressed, this, &AFSHeroCharacter::CameraShoulder);
	Input->BindAction(TEXT("CameraWide"), IE_Pressed, this, &AFSHeroCharacter::CameraWide);
	Input->BindAction(TEXT("CameraFirst"), IE_Pressed, this, &AFSHeroCharacter::CameraFirst);
	Input->BindAction(TEXT("CameraCycle"), IE_Pressed, this, &AFSHeroCharacter::CameraCycle);
	Input->BindAction(TEXT("SwapShoulder"), IE_Pressed, this, &AFSHeroCharacter::SwapShoulder);
	Input->BindAction(TEXT("ActPhoto"), IE_Pressed, this, &AFSHeroCharacter::ActPhoto);
	Input->BindAction(TEXT("ActCrier"), IE_Pressed, this, &AFSHeroCharacter::ActCrier);
	Input->BindAction(TEXT("ActEnvoyer"), IE_Pressed, this, &AFSHeroCharacter::Interact);
	Input->BindAction(TEXT("Reste"), IE_Pressed, this, &AFSHeroCharacter::Reste);
	Input->BindAction(TEXT("Cherche"), IE_Pressed, this, &AFSHeroCharacter::Cherche);
	Input->BindAction(TEXT("Non"), IE_Pressed, this, &AFSHeroCharacter::Non);
	Input->BindAction(TEXT("Bravo"), IE_Pressed, this, &AFSHeroCharacter::Bravo);
	Input->BindAction(TEXT("Montre"), IE_Pressed, this, &AFSHeroCharacter::Montre);
	Input->BindAction(TEXT("VaLaBas"), IE_Pressed, this, &AFSHeroCharacter::VaLaBas);
	// Ce qui est « caché dans l'ombre » n'apparaît qu'au faisceau de la lampe.
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(TEXT("Cache"))) { It->SetActorHiddenInGame(true); }
	}
	Input->BindAction(TEXT("Rappel"), IE_Pressed, this, &AFSHeroCharacter::Rappel);
	Input->BindAction(TEXT("Appel17"), IE_Pressed, this, &AFSHeroCharacter::Appel17);
	Input->BindAction(TEXT("Save"), IE_Pressed, this, &AFSHeroCharacter::Save);
	Input->BindAction(TEXT("Load"), IE_Pressed, this, &AFSHeroCharacter::QuickLoad);
	Input->BindAction(TEXT("Lampe"), IE_Pressed, this, &AFSHeroCharacter::ToggleTorch);
	Input->BindAction(TEXT("Inventaire"), IE_Pressed, this, &AFSHeroCharacter::OpenInventory);
	Input->BindAction(TEXT("Telephone"), IE_Pressed, this, &AFSHeroCharacter::OpenPhone);
	Input->BindAction(TEXT("Carnet"), IE_Pressed, this, &AFSHeroCharacter::ToggleNotebook);
	// Menu principal : les menus eux-mêmes sont pilotés par AFSPlayerController (touches captées en priorité).
	Input->BindAction(TEXT("Pause"), IE_Pressed, this, &AFSHeroCharacter::TogglePause);
	SetCameraMode(EFSCameraMode::Shoulder);

	if (UMaterialInstanceDynamic* Mat = PlaceholderBody->CreateDynamicMaterialInstance(0))
	{
		Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.16f, 0.29f, 0.52f));
	}
}

void AFSHeroCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float WantedArm = CameraMode == EFSCameraMode::First ? 0.f : FMath::Clamp(TargetArmLength + ZoomOffset, 120.f, 1100.f);
	CameraArm->TargetArmLength = FMath::FInterpTo(CameraArm->TargetArmLength, WantedArm, DeltaSeconds, 8.f);

	// La lampe torche révèle les acteurs taggés « Cache » qu'elle éclaire (scènes sombres, cabane).
	TorchCheck -= DeltaSeconds;
	if (IsTorchOn() && TorchCheck <= 0.f)
	{
		TorchCheck = 0.2f;
		FHitResult Hit;
		const FVector From = Camera->GetComponentLocation();
		FCollisionQueryParams Params(TEXT("FSLampe"), false, this);
		if (GetWorld()->LineTraceSingleByChannel(Hit, From, From + Camera->GetForwardVector() * 1500.f, ECC_Visibility, Params)
			&& Hit.GetActor() && Hit.GetActor()->ActorHasTag(TEXT("Cache")) && Hit.GetActor()->IsHidden())
		{
			Hit.GetActor()->SetActorHiddenInGame(false);
			if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->ShowToast(TEXT("Le faisceau de la lampe révèle quelque chose."), 3.f); }
		}
	}

	// « Cherche » : la première fois qu'Ariane trouve la balle, réplique DLG_P_TUTO_04.
	if (Ball && !bBallFoundSaid)
	{
		if (const AFSDogCharacter* Dog = FindDog())
		{
			if (Dog->IsHoldingNear(Ball->GetActorLocation()))
			{
				bBallFoundSaid = true;
				if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->PlayConversation({ FName(TEXT("DLG_P_TUTO_04")) }); }
				if (AFSDogCharacter* Happy = FindDog()) { Happy->SetMood(EFSDogMood::Happy, 4.f); }
			}
		}
	}
	CameraArm->SocketOffset = FMath::VInterpTo(CameraArm->SocketOffset, TargetSocketOffset, DeltaSeconds, 8.f);
}

void AFSHeroCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
	}
}

void AFSHeroCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
	}
}

void AFSHeroCharacter::StartRun()
{
	bRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AFSHeroCharacter::StopRun()
{
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFSHeroCharacter::SetCameraMode(EFSCameraMode NewMode)
{
	CameraMode = NewMode;
	ZoomOffset = 0.f;
	switch (NewMode)
	{
	case EFSCameraMode::Shoulder:
		TargetArmLength = 240.f;
		TargetSocketOffset = FVector(0.f, 55.f * ShoulderSide, 75.f);
		Camera->SetFieldOfView(70.f);
		break;
	case EFSCameraMode::Wide:
		TargetArmLength = 750.f;
		TargetSocketOffset = FVector(0.f, 0.f, 260.f);
		Camera->SetFieldOfView(65.f);
		break;
	case EFSCameraMode::First:
		TargetArmLength = 0.f;
		TargetSocketOffset = FVector(10.f, 0.f, 78.f); // yeux à ≈ 1,61 m
		Camera->SetFieldOfView(80.f);
		break;
	}
	PlaceholderBody->SetOwnerNoSee(NewMode == EFSCameraMode::First);
	static const TCHAR* Names[] = { TEXT("Vue épaule"), TEXT("Vue reculée"), TEXT("Vue subjective") };
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		if (!Hud->IsOnTitleScreen()) { Hud->ShowToast(Names[static_cast<int32>(NewMode)], 1.5f); }
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(10, 1.5f, FColor::White, Names[static_cast<int32>(NewMode)]);
	}
}

void AFSHeroCharacter::CameraCycle()
{
	SetCameraMode(static_cast<EFSCameraMode>((static_cast<int32>(CameraMode) + 1) % 3));
}

void AFSHeroCharacter::SwapShoulder()
{
	ShoulderSide = -ShoulderSide;
	if (CameraMode == EFSCameraMode::Shoulder)
	{
		SetCameraMode(EFSCameraMode::Shoulder);
	}
}

AFSPrologueDirector* AFSHeroCharacter::FindDirector() const
{
	return Cast<AFSPrologueDirector>(UGameplayStatics::GetActorOfClass(this, AFSPrologueDirector::StaticClass()));
}

AFSDogCharacter* AFSHeroCharacter::FindDog() const
{
	return Cast<AFSDogCharacter>(UGameplayStatics::GetActorOfClass(this, AFSDogCharacter::StaticClass()));
}

void AFSHeroCharacter::ActPhoto()
{
	// Pendant l'alerte, la photo est l'action ACT_PHOTO (avec ses règles) ; elle va aussi dans la galerie.
	if (AFSPrologueDirector* D = FindDirector())
	{
		const int32 Phase = D->GetPhaseIndex();
		if (Phase == 1 || Phase == 2)
		{
			const int32 Before = D->GetActionsLeft();
			D->TryAction(TEXT("ACT_PHOTO"));
			if (D->GetActionsLeft() < Before)
			{
				TakePhoto();
			}
			return;
		}
	}
	TakePhoto();
}

FString AFSHeroCharacter::DescribeView(bool& bVanVisible) const
{
	bVanVisible = false;
	const APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC || !PC->PlayerCameraManager)
	{
		return FString();
	}
	const FVector Eye = PC->PlayerCameraManager->GetCameraLocation();
	const FVector Fwd = PC->PlayerCameraManager->GetCameraRotation().Vector();
	struct FLabel { const TCHAR* Tag; const TCHAR* Text; };
	static const FLabel Labels[] = {
		{ TEXT("Fourgon"), TEXT("le fourgon blanc") }, { TEXT("K2"), TEXT("la femme au badge") }, { TEXT("Lila"), TEXT("Lila") },
		{ TEXT("dufau_placeholder"), TEXT("Marcel Dufau sur son banc") }, { TEXT("Herisson"), TEXT("un hérisson") },
		{ TEXT("school_main"), TEXT("l'école") }, { TEXT("tree_crown_square"), TEXT("le pin du square") },
		{ TEXT("PorteCles"), TEXT("un porte-clés renard au sol") }, { TEXT("Bracelet"), TEXT("des perles au sol") } };
	TArray<FString> Seen;
	FCollisionQueryParams Params(TEXT("FSPhoto"), false, this);
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->IsHidden()) { continue; }
		const FVector To = It->GetActorLocation() - Eye;
		const float Dist = To.Size();
		if (Dist > 4500.f || FVector::DotProduct(To.GetSafeNormal(), Fwd) < 0.85f) { continue; } // cône de ≈ 32°
		for (const FLabel& L : Labels)
		{
			if (!It->ActorHasTag(L.Tag)) { continue; }
			FHitResult Hit;
			const bool bBlocked = GetWorld()->LineTraceSingleByChannel(Hit, Eye, It->GetActorLocation(), ECC_Visibility, Params) && Hit.GetActor() != *It;
			if (!bBlocked)
			{
				Seen.AddUnique(L.Text);
				bVanVisible |= FCString::Strcmp(L.Tag, TEXT("Fourgon")) == 0;
			}
		}
		if (It->IsA(AFSDogCharacter::StaticClass())) { Seen.AddUnique(TEXT("Ariane")); }
	}
	return Seen.Num() ? FString::Printf(TEXT("On y voit %s."), *FString::Join(Seen, TEXT(", "))) : FString(TEXT("Rien de particulier dans le cadre."));
}

FString AFSHeroCharacter::TakePhoto()
{
	bool bVan = false;
	const FString Description = DescribeView(bVan);
	AFSPrologueDirector* D = FindDirector();
	const int32 Secs = D ? FMath::FloorToInt(D->GetPrologueSeconds()) : 0;
	const FString At = FString::Printf(TEXT("%02d:%02d"), 16 + (25 + Secs / 60) / 60, (25 + Secs / 60) % 60);
	if (UFSPhoneSubsystem* Phone = GetGameInstance()->GetSubsystem<UFSPhoneSubsystem>())
	{
		Phone->AddPhoto(At, (IsRunning() ? FString(TEXT("(floue) ")) : FString()) + Description);
	}
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		Hud->Flash();
		Hud->ShowToast(FString::Printf(TEXT("Photo enregistrée — %s"), *Description), 3.f);
	}
	return Description;
}

void AFSHeroCharacter::VaLaBas()
{
	// « Ariane, vas-y ! » vers l'endroit visé (hors de l'alerte, où E garde son rôle).
	const AFSPrologueDirector* D = FindDirector();
	const APlayerController* PC = Cast<APlayerController>(Controller);
	AFSDogCharacter* Dog = FindDog();
	if (!Dog || !PC || !PC->PlayerCameraManager || (D && D->GetPhaseIndex() >= 1 && D->GetPhaseIndex() <= 3))
	{
		return;
	}
	const FVector Eye = PC->PlayerCameraManager->GetCameraLocation();
	FHitResult Hit;
	FCollisionQueryParams Params(TEXT("FSVaLaBas"), false, this);
	Params.AddIgnoredActor(Dog);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Eye, Eye + PC->PlayerCameraManager->GetCameraRotation().Vector() * 3000.f, ECC_Visibility, Params))
	{
		Dog->SendTo(Hit.Location);
		if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->ShowToast(TEXT("« Ariane, vas-y ! »"), 1.5f); }
	}
}


void AFSHeroCharacter::ActCrier()
{
	if (AFSPrologueDirector* D = FindDirector()) { D->TryAction(TEXT("ACT_CRIER")); }
}

void AFSHeroCharacter::ActEnvoyer()
{
	if (AFSPrologueDirector* D = FindDirector()) { D->TryAction(TEXT("ACT_ENVOYER")); }
}

void AFSHeroCharacter::Rappel()
{
	if (AFSDogCharacter* Dog = FindDog())
	{
		Dog->Recall();
		if (AFSHUD* Hud = AFSHUD::Get(this))
		{
			if (const UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>())
			{
				Hud->ShowToast(M->GetUIText(TEXT("UI_ORDRE_AU_PIED"), TEXT("Ariane : « Au pied. »")), 1.5f);
			}
		}
	}
	if (Ball)
	{
		Ball->Destroy();
		Ball = nullptr;
	}
}

void AFSHeroCharacter::Appel17()
{
	if (AFSPrologueDirector* D = FindDirector()) { D->CallPolice(); }
}

void AFSHeroCharacter::Save()
{
	const bool bOk = UFSSaveGame::SaveSlot(this, 0);
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		Hud->ShowToast(bOk ? TEXT("Sauvegarde rapide enregistrée (F9 pour la recharger)") : TEXT("Échec de la sauvegarde"), 2.5f);
	}
}

void AFSHeroCharacter::QuickLoad()
{
	const bool bOk = UFSSaveGame::LoadSlot(this, 0);
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		if (bOk) { Hud->OnGameLoaded(); }
		Hud->ShowToast(bOk ? TEXT("Sauvegarde rapide rechargée") : TEXT("Aucune sauvegarde rapide"), 2.5f);
	}
}

void AFSHeroCharacter::ToggleNotebook()
{
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		if (!Hud->IsOnTitleScreen()) { Hud->ToggleNotebook(); }
	}
}

// --- Regard et caméra ------------------------------------------------------------------------

void AFSHeroCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * FSSettings::MouseSensitivity());
}

void AFSHeroCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * FSSettings::MouseSensitivity() * (FSSettings::InvertY() ? -1.f : 1.f));
}

void AFSHeroCharacter::TurnRate(float Value)
{
	// Stick droit : vitesse constante quelle que soit la cadence d'images.
	if (Value != 0.f)
	{
		AddControllerYawInput(Value * GamepadTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AFSHeroCharacter::LookUpRate(float Value)
{
	if (Value != 0.f)
	{
		AddControllerPitchInput(Value * 0.7f * GamepadTurnRate * GetWorld()->GetDeltaSeconds() * (FSSettings::InvertY() ? -1.f : 1.f));
	}
}

void AFSHeroCharacter::Zoom(float Value)
{
	if (Value != 0.f && CameraMode != EFSCameraMode::First)
	{
		ZoomOffset = FMath::Clamp(ZoomOffset - 60.f * Value, -300.f, 500.f);
	}
}

// --- Interactions et ordres à Ariane ---------------------------------------------------------

int32 AFSHeroCharacter::FindInteraction(AActor** PickupTarget) const
{
	if (const AFSPrologueDirector* D = FindDirector())
	{
		if (D->CanWave())
		{
			return 1;
		}
		if (D->GetPhaseIndex() >= 1 && D->GetPhaseIndex() <= 3)
		{
			return 0; // pendant l'alerte et le départ, E sert à « Ariane, vas-y ! »
		}
	}
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		const float Dist = FVector::Dist2D(It->GetActorLocation(), GetActorLocation());
		if (!bTalkedToDufau && It->ActorHasTag(TEXT("dufau_placeholder")) && Dist < 350.f)
		{
			return 2;
		}
		if ((It->ActorHasTag(TEXT("Ficelle")) || It->ActorHasTag(TEXT("Corde"))) && !It->IsHidden() && Dist < 200.f)
		{
			if (PickupTarget) { *PickupTarget = *It; }
			return 4;
		}
		// Preuve à ramasser : acteur visible portant le tag « Preuve » et l'ID de l'indice (CLU_…).
		if (It->ActorHasTag(TEXT("Preuve")) && !It->IsHidden() && Dist < 220.f)
		{
			if (PickupTarget) { *PickupTarget = *It; }
			return 3;
		}
	}
	return 0;
}

namespace
{
	FName ClueTagOf(const AActor* Actor)
	{
		for (const FName& Tag : Actor->Tags)
		{
			if (Tag.ToString().StartsWith(TEXT("CLU_"))) { return Tag; }
		}
		return NAME_None;
	}
}

FString AFSHeroCharacter::GetInteractionPrompt() const
{
	const UFSMissionSubsystem* M = GetGameInstance() ? GetGameInstance()->GetSubsystem<UFSMissionSubsystem>() : nullptr;
	if (!M)
	{
		return FString();
	}
	AActor* Target = nullptr;
	switch (FindInteraction(&Target))
	{
	case 1: return M->GetUIText(TEXT("UI_INVITE_COUCOU"), TEXT("E : répondre au coucou de Lila"));
	case 2: return M->GetUIText(TEXT("UI_INVITE_DUFAU"), TEXT("E : saluer Marcel Dufau"));
	case 4: return TEXT("E : couper avec l'Opinel");
	case 3: return Target ? FString::Printf(TEXT("E : ramasser (%s)%s"), *M->GetClueName(ClueTagOf(Target)), bGloves ? TEXT("") : TEXT(" — gants conseillés")) : FString();
	default: return FString();
	}
}

void AFSHeroCharacter::Interact()
{
	AFSPrologueDirector* D = FindDirector();
	if (D && D->GetPhaseIndex() >= 1 && D->GetPhaseIndex() <= 3)
	{
		ActEnvoyer(); // fenêtre d'action : « Ariane, vas-y ! »
		return;
	}
	AActor* Target = nullptr;
	switch (FindInteraction(&Target))
	{
	case 1:
		if (D) { D->TryWave(); }
		break;
	case 2:
		bTalkedToDufau = true;
		if (AFSHUD* Hud = AFSHUD::Get(this))
		{
			Hud->PlayConversation({ FName(TEXT("DLG_P_DUFAU_01")), FName(TEXT("DLG_P_DUFAU_02")), FName(TEXT("DLG_P_DUFAU_03")),
				FName(TEXT("DLG_P_DUFAU_04")), FName(TEXT("DLG_P_DUFAU_05")) });
		}
		break;
	case 3:
		if (Target)
		{
			AFSHUD* Hud = AFSHUD::Get(this);
			if (!bGloves)
			{
				if (Hud) { Hud->ShowToast(TEXT("Enfile d'abord tes gants (inventaire, touche I) pour ne pas laisser d'empreintes."), 3.5f); }
				break;
			}
			const FName Clue = ClueTagOf(Target);
			Target->SetActorHiddenInGame(true);
			Target->SetActorEnableCollision(false);
			PickedUp.AddUnique(Clue);
			if (UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>()) { M->Grant(Clue); }
			if (Hud) { Hud->ShowToast(TEXT("Rangé dans le sac, rubrique Preuves (I)"), 3.f); }
		}
		break;
	case 4:
		if (Target)
		{
			Target->SetActorHiddenInGame(true);
			Target->SetActorEnableCollision(false);
			if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->ShowToast(TEXT("Un coup d'Opinel : c'est coupé."), 2.5f); }
		}
		break;
	default:
		break;
	}
}

void AFSHeroCharacter::Reste()
{
	if (AFSDogCharacter* Dog = FindDog())
	{
		Dog->Stay();
		if (AFSHUD* Hud = AFSHUD::Get(this))
		{
			if (const UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>())
			{
				Hud->ShowToast(M->GetUIText(TEXT("UI_ORDRE_RESTE"), TEXT("Ariane : « Reste. »")), 1.5f);
			}
		}
	}
}

void AFSHeroCharacter::Cherche()
{
	// Balle lancée devant l'héroïne, seulement pendant la promenade (avant l'alerte).
	const AFSPrologueDirector* D = FindDirector();
	AFSDogCharacter* Dog = FindDog();
	if (!Dog || (D && D->GetPhaseIndex() != 0))
	{
		return;
	}
	const FRotator Yaw(0.f, Controller ? Controller->GetControlRotation().Yaw : GetActorRotation().Yaw, 0.f);
	FVector Target = GetActorLocation() + FRotationMatrix(Yaw).GetUnitAxis(EAxis::X) * 900.f;
	FHitResult Hit;
	FCollisionQueryParams Params(TEXT("FSBalle"), false, this);
	Params.AddIgnoredActor(Dog);
	// La balle s'arrête devant un mur ou une haie, puis tombe au sol.
	if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), Target, ECC_Visibility, Params))
	{
		Target = Hit.Location - FRotationMatrix(Yaw).GetUnitAxis(EAxis::X) * 60.f;
	}
	if (GetWorld()->LineTraceSingleByChannel(Hit, Target + FVector(0.f, 0.f, 200.f), Target - FVector(0.f, 0.f, 500.f), ECC_Visibility, Params))
	{
		Target = Hit.Location + FVector(0.f, 0.f, 4.f);
	}
	if (!Ball)
	{
		FActorSpawnParameters Spawn;
		Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AStaticMeshActor* NewBall = GetWorld()->SpawnActor<AStaticMeshActor>(Target, FRotator::ZeroRotator, Spawn);
		if (NewBall)
		{
			NewBall->SetMobility(EComponentMobility::Movable);
			if (UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere")))
			{
				NewBall->GetStaticMeshComponent()->SetStaticMesh(Sphere);
			}
			NewBall->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			NewBall->SetActorScale3D(FVector(0.08f));
			if (UMaterialInstanceDynamic* Mat = NewBall->GetStaticMeshComponent()->CreateDynamicMaterialInstance(0))
			{
				Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.85f, 0.95f, 0.1f)); // balle de tennis
			}
			Ball = NewBall;
		}
	}
	else
	{
		Ball->SetActorLocation(Target);
	}
	Dog->SendTo(Target);
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		if (const UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>())
		{
			Hud->ShowToast(M->GetUIText(TEXT("UI_ORDRE_CHERCHE"), TEXT("Ariane : « Cherche ! »")), 1.5f);
		}
	}
}


// --- Menus -----------------------------------------------------------------------------------

void AFSHeroCharacter::TogglePause()
{
	if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->OpenPage(EFSPage::Main); }
}

void AFSHeroCharacter::OpenInventory()
{
	if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->OpenPage(EFSPage::Inventory); }
}

void AFSHeroCharacter::OpenPhone()
{
	if (AFSHUD* Hud = AFSHUD::Get(this)) { Hud->OpenPage(EFSPage::Phone); }
}

// --- Inventaire ------------------------------------------------------------------------------

const TArray<FFSBagItem>& AFSHeroCharacter::BagItems()
{
	static const TArray<FFSBagItem> Items = {
		{ TEXT("TELEPHONE"), TEXT("Téléphone"), TEXT("Appels, répertoire, journal, messages (avec réponses), mails, notifications, appareil photo et galerie. Touche O pour l'ouvrir, F pour une photo.") },
		{ TEXT("BONBONS"), TEXT("Bonbons pour Ariane"), TEXT("Des friandises au poulet. Ariane revient au pied pour une seule d'entre elles.") },
		{ TEXT("OPINEL"), TEXT("Petit Opinel"), TEXT("Un couteau pliant à virole, lame de 6 cm. Approche-toi d'une ficelle ou d'une corde : E pour la couper.") },
		{ TEXT("GANTS"), TEXT("Gants"), TEXT("Une paire de gants fins. À enfiler avant de ramasser une preuve, pour ne pas y laisser ses empreintes.") },
		{ TEXT("LAMPE"), TEXT("Lampe torche"), TEXT("Petite lampe à LED. Touche L pour l'allumer ou l'éteindre. Son faisceau révèle ce qui se cache dans l'ombre.") },
	};
	return Items;
}

bool AFSHeroCharacter::IsTorchOn() const
{
	return Torch && Torch->IsVisible();
}

void AFSHeroCharacter::SetTorch(bool bOn)
{
	if (Torch) { Torch->SetVisibility(bOn); }
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		Hud->ShowToast(bOn ? TEXT("Lampe torche allumée") : TEXT("Lampe torche éteinte"), 1.5f);
	}
}

FString AFSHeroCharacter::BagItemState(const FString& Id) const
{
	if (Id == TEXT("BONBONS")) { return FString::Printf(TEXT("%d restant%s"), Candies, Candies > 1 ? TEXT("s") : TEXT("")); }
	if (Id == TEXT("GANTS")) { return bGloves ? TEXT("enfilés") : TEXT("dans le sac"); }
	if (Id == TEXT("LAMPE")) { return IsTorchOn() ? TEXT("allumée") : TEXT("éteinte"); }
	if (Id == TEXT("TELEPHONE"))
	{
		UFSPhoneSubsystem* Phone = GetGameInstance() ? GetGameInstance()->GetSubsystem<UFSPhoneSubsystem>() : nullptr;
		const int32 Unread = Phone ? Phone->CountUnread(TEXT("message")) + Phone->CountUnread(TEXT("mail")) + Phone->CountUnread(TEXT("notification")) : 0;
		return Unread > 0 ? FString::Printf(TEXT("%d nouveauté%s"), Unread, Unread > 1 ? TEXT("s") : TEXT("")) : FString();
	}
	return FString();
}

FString AFSHeroCharacter::UseBagItem(const FString& Id)
{
	if (Id == TEXT("TELEPHONE"))
	{
		OpenPhone();
		return FString();
	}
	if (Id == TEXT("BONBONS"))
	{
		AFSDogCharacter* Dog = FindDog();
		if (Candies <= 0) { return TEXT("Le sachet est vide."); }
		if (!Dog || FVector::Dist2D(Dog->GetActorLocation(), GetActorLocation()) > 1500.f)
		{
			return TEXT("Ariane est trop loin pour voir le bonbon.");
		}
		--Candies;
		Dog->Recall();
		Dog->SetMood(EFSDogMood::Happy, 4.f);
		return FString::Printf(TEXT("Ariane revient au pied et croque sa friandise. (%d restant%s)"), Candies, Candies > 1 ? TEXT("s") : TEXT(""));
	}
	if (Id == TEXT("OPINEL"))
	{
		AActor* Target = nullptr;
		return FindInteraction(&Target) == 4 ? TEXT("Approche-toi et appuie sur E pour couper.") : TEXT("Rien à couper ici. Tu ranges l'Opinel.");
	}
	if (Id == TEXT("GANTS"))
	{
		bGloves = !bGloves;
		return bGloves ? TEXT("Tu enfiles tes gants.") : TEXT("Tu ranges tes gants dans le sac.");
	}
	if (Id == TEXT("LAMPE"))
	{
		SetTorch(!IsTorchOn());
	}
	return FString();
}

FString AFSHeroCharacter::UseEvidence(FName ClueId)
{
	const UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>();
	if (M && M->GetClueKind(ClueId) == TEXT("objet"))
	{
		return SniffEvidence(ClueId);
	}
	return M ? M->GetClueFact(ClueId) : FString();
}

FString AFSHeroCharacter::SniffEvidence(FName ClueId)
{
	AFSDogCharacter* Dog = FindDog();
	const UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>();
	if (!Dog || !M)
	{
		return FString();
	}
	if (FVector::Dist2D(Dog->GetActorLocation(), GetActorLocation()) > 400.f)
	{
		return TEXT("Ariane est trop loin : rappelle-la (R) pour lui faire sentir l'objet.");
	}
	// Piste correspondante : acteurs taggés « Piste » et l'ID de l'indice, dans l'ordre « Ordre_N » ; « FinPiste » au bout.
	struct FPoint { int32 Order; FVector Location; bool bEnd; };
	TArray<FPoint> Points;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (!It->ActorHasTag(TEXT("Piste")) || !It->ActorHasTag(ClueId)) { continue; }
		int32 Order = 0;
		for (const FName& Tag : It->Tags)
		{
			const FString T = Tag.ToString();
			if (T.StartsWith(TEXT("Ordre_"))) { Order = FCString::Atoi(*T.RightChop(6)); }
		}
		Points.Add({ Order, It->GetActorLocation(), It->ActorHasTag(TEXT("FinPiste")) });
	}
	Points.Sort([](const FPoint& A, const FPoint& B) { return A.Order < B.Order; });
	TArray<FVector> Path;
	bool bFound = false;
	for (const FPoint& P : Points) { Path.Add(P.Location); bFound |= P.bEnd; }
	Dog->Recall();
	Dog->Track(Path, bFound);
	return FString::Printf(TEXT("Tu fais sentir « %s » à Ariane : « Sens… Cherche ! »"), *M->GetClueName(ClueId));
}

void AFSHeroCharacter::WriteState(UFSSaveGame& Save) const
{
	Save.HeroTransform = GetActorTransform();
	Save.ControlRotation = Controller ? Controller->GetControlRotation() : GetActorRotation();
	Save.CameraMode = static_cast<uint8>(CameraMode);
	Save.Candies = Candies;
	Save.bGloves = bGloves;
	Save.bTorch = IsTorchOn();
	Save.bTalkedToDufau = bTalkedToDufau;
	Save.PickedUp = PickedUp;
}

void AFSHeroCharacter::ReadState(const UFSSaveGame& Save)
{
	SetActorTransform(Save.HeroTransform, false, nullptr, ETeleportType::TeleportPhysics);
	GetCharacterMovement()->StopMovementImmediately();
	if (Controller) { Controller->SetControlRotation(Save.ControlRotation); }
	SetCameraMode(static_cast<EFSCameraMode>(FMath::Min<uint8>(Save.CameraMode, 2)));
	Candies = Save.Candies;
	bGloves = Save.bGloves;
	if (Torch) { Torch->SetVisibility(Save.bTorch); }
	bTalkedToDufau = Save.bTalkedToDufau;
	PickedUp = Save.PickedUp;
	// Les preuves déjà ramassées disparaissent du décor.
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		for (const FName& Id : PickedUp)
		{
			if (It->ActorHasTag(Id) && It->ActorHasTag(TEXT("Preuve")))
			{
				It->SetActorHiddenInGame(true);
				It->SetActorEnableCollision(false);
			}
		}
	}
	if (Ball) { Ball->Destroy(); Ball = nullptr; }
}

void AFSHeroCharacter::Non()
{
	if (AFSDogCharacter* Dog = FindDog()) { Dog->Scold(); }
}

void AFSHeroCharacter::Bravo()
{
	if (AFSDogCharacter* Dog = FindDog()) { Dog->Praise(); }
}

void AFSHeroCharacter::Montre()
{
	if (AFSDogCharacter* Dog = FindDog()) { Dog->Show(); }
}
