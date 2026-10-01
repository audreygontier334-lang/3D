#include "FSDogCharacter.h"
#include "FauxSemblants.h"
#include "FSHUD.h"
#include "FSMissionSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	UStaticMeshComponent* MakePart(AActor* Owner, const TCHAR* Name, USceneComponent* Parent, UStaticMesh* Mesh, const FVector& Location, const FVector& Scale)
	{
		UStaticMeshComponent* Part = Owner->CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Part->SetupAttachment(Parent);
		Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Part->SetStaticMesh(Mesh);
		Part->SetRelativeLocation(Location);
		Part->SetRelativeScale3D(Scale);
		return Part;
	}
}

AFSDogCharacter::AFSDogCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(28.f, 32.f);
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 720.f, 0.f);
	Move->MaxWalkSpeed = TrotSpeed;
	// Saute très haut (plus de 2 m : v² / 2g ≈ 2,6 m) et rampe sous les obstacles bas.
	Move->JumpZVelocity = 720.f;
	Move->AirControl = 0.6f;
	Move->GetNavAgentPropertiesRef().bCanCrouch = true;
	Move->SetCrouchedHalfHeight(16.f);
	Move->MaxWalkSpeedCrouched = 180.f;
	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* CubeMesh = Cube.Succeeded() ? Cube.Object : nullptr;

	Visual = CreateDefaultSubobject<USceneComponent>(TEXT("Visual"));
	Visual->SetupAttachment(GetCapsuleComponent());

	PlaceholderBody = MakePart(this, TEXT("PlaceholderBody"), Visual, CubeMesh, FVector::ZeroVector, FVector(0.95f, 0.36f, 0.6f)); // ≈ 35 kg, 60 cm au garrot

	// Oreilles : pivot à la base, sur le haut de la tête ; au repos, la droite dressée et la gauche tombante.
	EarRightPivot = CreateDefaultSubobject<USceneComponent>(TEXT("EarRightPivot"));
	EarRightPivot->SetupAttachment(Visual);
	EarRightPivot->SetRelativeLocation(FVector(40.f, 10.f, 30.f));
	EarRight = MakePart(this, TEXT("EarRight"), EarRightPivot, CubeMesh, FVector(0.f, 0.f, 7.f), FVector(0.04f, 0.07f, 0.14f));
	EarLeftPivot = CreateDefaultSubobject<USceneComponent>(TEXT("EarLeftPivot"));
	EarLeftPivot->SetupAttachment(Visual);
	EarLeftPivot->SetRelativeLocation(FVector(40.f, -10.f, 30.f));
	EarLeft = MakePart(this, TEXT("EarLeft"), EarLeftPivot, CubeMesh, FVector(0.f, 0.f, 7.f), FVector(0.04f, 0.07f, 0.14f));

	// Queue : pivot à la racine, derrière le dos.
	TailPivot = CreateDefaultSubobject<USceneComponent>(TEXT("TailPivot"));
	TailPivot->SetupAttachment(Visual);
	TailPivot->SetRelativeLocation(FVector(-47.f, 0.f, 22.f));
	Tail = MakePart(this, TEXT("Tail"), TailPivot, CubeMesh, FVector(-15.f, 0.f, 0.f), FVector(0.30f, 0.05f, 0.05f));
}

void AFSDogCharacter::BeginPlay()
{
	Super::BeginPlay();
	const FLinearColor Fawn(0.62f, 0.44f, 0.25f);  // beige fauve
	const FLinearColor Black(0.05f, 0.04f, 0.04f); // masque et oreilles noirs
	UStaticMeshComponent* Parts[] = { PlaceholderBody, Tail, EarRight, EarLeft };
	for (UStaticMeshComponent* Part : Parts)
	{
		if (UMaterialInstanceDynamic* Mat = Part ? Part->CreateDynamicMaterialInstance(0) : nullptr)
		{
			Mat->SetVectorParameterValue(TEXT("Color"), (Part == EarRight || Part == EarLeft) ? Black : Fawn);
		}
	}
}

void AFSDogCharacter::Say(const TCHAR* LineId)
{
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		Hud->PlayConversation({ FName(LineId) });
	}
}

void AFSDogCharacter::Signal(const TCHAR* UIId, const TCHAR* Fallback)
{
	// Langage d'Ariane (UI_CARNET_CHIENNE_…), affiché comme ce que l'héroïne lit dans son attitude.
	const UFSMissionSubsystem* M = GetGameInstance() ? GetGameInstance()->GetSubsystem<UFSMissionSubsystem>() : nullptr;
	if (AFSHUD* Hud = AFSHUD::Get(this))
	{
		Hud->ShowToast(FString::Printf(TEXT("Ariane — %s"), *(M ? M->GetUIText(FName(UIId), Fallback) : FString(Fallback))), 5.f);
	}
}

void AFSDogCharacter::Track(const TArray<FVector>& Points, bool bFoundAtEnd)
{
	if (Points.Num() == 0)
	{
		Signal(TEXT("UI_ECHEC_PISTE_VIDE"), TEXT("Ariane ne trouve rien à suivre avec cet objet."));
		SetMood(EFSDogMood::Neutral, 2.f);
		return;
	}
	TrailPoints = Points;
	TrailIndex = 0;
	bTrailSucceeds = bFoundAtEnd;
	bSitting = false;
	State = EState::Tracking;
	GetCharacterMovement()->MaxWalkSpeed = TrotSpeed * 0.75f;
	SetMood(EFSDogMood::Alert, 60.f);
	Signal(TEXT("UI_CARNET_CHIENNE_1"), TEXT("Truffe au sol, allure régulière : elle suit une piste fraîche."));
}

bool AFSDogCharacter::HasFoundNear(const FVector& Point, float Radius) const
{
	return State == EState::Found && FVector::Dist2D(GetActorLocation(), Point) <= Radius;
}

void AFSDogCharacter::SetMood(EFSDogMood NewMood, float Seconds)
{
	Mood = NewMood;
	MoodLeft = Seconds;
}

// --- Ordres ------------------------------------------------------------------------------------

void AFSDogCharacter::SendTo(const FVector& Target)
{
	SentTarget = Target;
	State = EState::Sent;
	bSitting = false;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	SetMood(EFSDogMood::Alert, 6.f);
}

void AFSDogCharacter::Recall()
{
	State = EState::Follow;
	bSitting = false;
	FreezeLeft = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = TrotSpeed;
}

void AFSDogCharacter::Stay()
{
	State = EState::Staying;
	bSitting = true;
	GetCharacterMovement()->StopMovementImmediately();
}

void AFSDogCharacter::Scold()
{
	Recall();
	GetCharacterMovement()->StopMovementImmediately();
	SetMood(EFSDogMood::Sad, 6.f);
	Say(TEXT("DLG_P_ARIANE_NON"));
}

void AFSDogCharacter::Praise()
{
	SetMood(EFSDogMood::Happy, 4.f);
	Say(TEXT("DLG_P_ARIANE_BRAVO"));
}

void AFSDogCharacter::Show()
{
	if (bHasPointOfInterest)
	{
		LeadTo(PointOfInterest);
		return;
	}
	TiltLeft = 2.f;
	Say(TEXT("DLG_P_ARIANE_MONTRE_RIEN"));
}

void AFSDogCharacter::LeadTo(const FVector& Point)
{
	PointOfInterest = Point;
	bHasPointOfInterest = true;
	State = EState::Talking;
	TalkTime = 0.f;
	LeadTime = 0.f;
	bLeadLineSaid = false;
	bWaitingForHero = false;
	GetCharacterMovement()->MaxWalkSpeed = TrotSpeed;
	SetMood(EFSDogMood::Talking, 30.f);
}

void AFSDogCharacter::Bolt(const FVector& Point)
{
	SendTo(Point);
	Say(TEXT("DLG_P_ARIANE_PISTE"));
}

bool AFSDogCharacter::IsHoldingNear(const FVector& Point, float Radius) const
{
	return State == EState::Holding && FVector::Dist2D(GetActorLocation(), Point) <= Radius;
}

// --- Sens : danger, hérisson, protection ---------------------------------------------------------

bool AFSDogCharacter::IsThreat(const AActor* Actor) const
{
	return Actor && !Actor->IsHidden() && (Actor->ActorHasTag(TEXT("Menace")) || Actor->ActorHasTag(TEXT("K2")));
}

void AFSDogCharacter::Sense(float DeltaSeconds)
{
	GrowlCooldown -= DeltaSeconds;
	ProtectCooldown -= DeltaSeconds;
	JumpLineCooldown -= DeltaSeconds;
	SenseTimer -= DeltaSeconds;
	if (SenseTimer > 0.f)
	{
		return;
	}
	SenseTimer = 0.4f;
	const APawn* Hero = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Hero)
	{
		return;
	}
	ProtectFrom = nullptr;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* A = *It;
		if (A == this || A->IsHidden())
		{
			continue;
		}
		const float ToDog = FVector::Dist2D(A->GetActorLocation(), GetActorLocation());
		// Fausse alerte : le hérisson du square.
		if (A->ActorHasTag(TEXT("Herisson")))
		{
			if (!bHedgehogGrowled && ToDog < 900.f && State == EState::Follow)
			{
				bHedgehogGrowled = true;
				SetMood(EFSDogMood::Alert, 5.f);
				FreezeLeft = 4.f;
				FreezeLook = A->GetActorLocation();
				Say(TEXT("DLG_P_ARIANE_HERISSON_01"));
			}
			else if (bHedgehogGrowled && !bHedgehogExplained && FVector::Dist2D(A->GetActorLocation(), Hero->GetActorLocation()) < 300.f)
			{
				bHedgehogExplained = true;
				SetMood(EFSDogMood::Happy, 3.f);
				Say(TEXT("DLG_P_ARIANE_HERISSON_02"));
			}
			continue;
		}
		// Protectrice : elle s'interpose entre l'héroïne et une menace proche.
		if (IsThreat(A) && FVector::Dist2D(A->GetActorLocation(), Hero->GetActorLocation()) < 700.f)
		{
			ProtectFrom = A;
			if (ProtectCooldown <= 0.f)
			{
				ProtectCooldown = 25.f;
				SetMood(EFSDogMood::Alert, 6.f);
				Say(TEXT("DLG_P_ARIANE_PROTEGE"));
			}
			continue;
		}
		// Un enfant menacé à portée : elle grogne (sans modifier la chronologie du prologue).
		if (IsThreat(A) && GrowlCooldown <= 0.f && ToDog < 1800.f)
		{
			for (TActorIterator<AActor> Child(GetWorld()); Child; ++Child)
			{
				if ((Child->ActorHasTag(TEXT("Lila")) || Child->ActorHasTag(TEXT("Enfant"))) && !Child->IsHidden()
					&& FVector::Dist2D(Child->GetActorLocation(), A->GetActorLocation()) < 400.f)
				{
					GrowlCooldown = 20.f;
					SetMood(EFSDogMood::Alert, 5.f);
					FreezeLeft = 2.f;
					FreezeLook = A->GetActorLocation();
					Say(TEXT("DLG_P_ARIANE_GROGNE"));
					break;
				}
			}
		}
		// Quelque chose d'anormal (acteur taggé « Danger »).
		if (A->ActorHasTag(TEXT("Danger")) && GrowlCooldown <= 0.f && ToDog < 800.f)
		{
			GrowlCooldown = 20.f;
			SetMood(EFSDogMood::Alert, 5.f);
			FreezeLeft = 2.f;
			FreezeLook = A->GetActorLocation();
			Say(TEXT("DLG_P_ARIANE_GROGNE"));
		}
	}
}

// --- Obstacles : sauter, ramper, ouvrir les portes ----------------------------------------------

void AFSDogCharacter::AvoidObstacles(const FVector& Direction)
{
	UCharacterMovementComponent* Move = GetCharacterMovement();
	if (!Move->IsMovingOnGround() || Direction.IsNearlyZero())
	{
		return;
	}
	const FVector Base = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	const FVector Dir = Direction.GetSafeNormal2D();
	FCollisionQueryParams Params(TEXT("FSAriane"), false, this);
	if (const APawn* Hero = UGameplayStatics::GetPlayerPawn(this, 0)) { Params.AddIgnoredActor(Hero); }
	auto Blocked = [&](float Height, float Reach, FHitResult& Hit)
	{
		const FVector From = Base + FVector(0.f, 0.f, Height);
		return GetWorld()->LineTraceSingleByChannel(Hit, From, From + Dir * Reach, ECC_Visibility, Params);
	};
	FHitResult Knee, Head, High;
	const bool bKnee = Blocked(20.f, 90.f, Knee);
	const bool bHead = Blocked(62.f, 90.f, Head);

	// Porte : elle se dresse sur la poignée et l'ouvre.
	const FHitResult& Any = bKnee ? Knee : Head;
	if ((bKnee || bHead) && Any.GetActor() && Any.GetActor()->ActorHasTag(TEXT("Porte")) && !Any.GetActor()->ActorHasTag(TEXT("Ouverte")))
	{
		AActor* Door = Any.GetActor();
		Door->Tags.Add(TEXT("Ouverte"));
		Door->AddActorLocalRotation(FRotator(0.f, 85.f, 0.f));
		Door->SetActorEnableCollision(false);
		if (!bDoorLineSaid) { bDoorLineSaid = true; Say(TEXT("DLG_P_ARIANE_PORTE")); }
		return;
	}
	if (bKnee && bHead && !Blocked(235.f, 110.f, High))
	{
		// Obstacle de moins de 2,3 m : elle saute par-dessus.
		Jump();
		if (JumpLineCooldown <= 0.f) { JumpLineCooldown = 30.f; Say(TEXT("DLG_P_ARIANE_SAUT")); }
	}
	else if (bHead && !bKnee)
	{
		Crouch(); // passage bas : elle rampe
	}
	else if (!bHead && bIsCrouched)
	{
		UnCrouch();
	}
}

// --- Expressions -------------------------------------------------------------------------------

void AFSDogCharacter::FaceTowards(const FVector& Point, float DeltaSeconds)
{
	FVector To = Point - GetActorLocation();
	To.Z = 0.f;
	if (!To.IsNearlyZero())
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), To.Rotation(), DeltaSeconds, 6.f));
	}
}

void AFSDogCharacter::AnimateExpressions(float DeltaSeconds)
{
	Clock += DeltaSeconds;
	float WantRight = 0.f, WantLeft = 60.f, WantLeftRoll = 50.f, WantTail = 20.f, Wag = 4.f, WagSpeed = 2.f, Hop = 0.f;
	switch (Mood)
	{
	case EFSDogMood::Alert:   WantRight = 0.f;   WantLeft = 0.f;   WantLeftRoll = 0.f;  WantTail = 60.f;  Wag = 0.f;  break;
	case EFSDogMood::Happy:   WantRight = 15.f;  WantLeft = 55.f;  WantLeftRoll = 40.f; WantTail = 35.f;  Wag = 35.f; WagSpeed = 16.f; Hop = 9.f; break;
	case EFSDogMood::Sad:     WantRight = -75.f; WantLeft = -75.f; WantLeftRoll = 0.f;  WantTail = -45.f; Wag = 0.f;  break;
	case EFSDogMood::Talking: WantRight = 0.f;   WantLeft = 35.f;  WantLeftRoll = 25.f; WantTail = 30.f;  Wag = 25.f; WagSpeed = 12.f; break;
	default: break;
	}
	const float K = FMath::Clamp(DeltaSeconds * 10.f, 0.f, 1.f);
	EarRightPitch = FMath::Lerp(EarRightPitch, WantRight, K);
	EarLeftPitch = FMath::Lerp(EarLeftPitch, WantLeft, K);
	EarLeftRoll = FMath::Lerp(EarLeftRoll, WantLeftRoll, K);
	TailPitch = FMath::Lerp(TailPitch, WantTail, K);
	TailYaw = Wag * FMath::Sin(Clock * WagSpeed);
	BodyPitch = FMath::Lerp(BodyPitch, bSitting ? 18.f : 0.f, K);
	Bounce = Hop > 0.f ? Hop * FMath::Abs(FMath::Sin(Clock * 9.f)) : FMath::Lerp(Bounce, Mood == EFSDogMood::Sad ? -4.f : 0.f, K);

	// Tête penchée (« Montre » sans rien à montrer) : les deux oreilles s'inclinent.
	TiltLeft = FMath::Max(0.f, TiltLeft - DeltaSeconds);
	const float Tilt = TiltLeft > 0.f ? 18.f : 0.f;

	// Oreille gauche tombante : bascule vers l'avant (tangage positif) et vers l'extérieur.
	EarRightPivot->SetRelativeRotation(FRotator(-EarRightPitch, 0.f, Tilt));
	EarLeftPivot->SetRelativeRotation(FRotator(-EarLeftPitch, 0.f, -EarLeftRoll + Tilt));
	// Queue : relevée (à l'affût), basse (triste), remue (contente).
	TailPivot->SetRelativeRotation(FRotator(TailPitch, TailYaw, 0.f));
	// Assise : l'arrière-train s'abaisse ; sautille quand elle est contente.
	Visual->SetRelativeRotation(FRotator(BodyPitch, 0.f, 0.f));
	Visual->SetRelativeLocation(FVector(0.f, 0.f, Bounce - (bSitting ? 6.f : 0.f)));
}

// --- Boucle ------------------------------------------------------------------------------------

void AFSDogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AnimateExpressions(DeltaSeconds);
	MoodLeft -= DeltaSeconds;
	if (MoodLeft <= 0.f && State != EState::Talking && State != EState::Leading)
	{
		Mood = (State == EState::Sent || State == EState::Holding) ? EFSDogMood::Alert : EFSDogMood::Neutral;
	}

	const APawn* Hero = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Hero)
	{
		return;
	}
	Sense(DeltaSeconds);

	// Arrêt bref pour fixer quelque chose (hérisson, menace).
	if (FreezeLeft > 0.f && State == EState::Follow)
	{
		FreezeLeft -= DeltaSeconds;
		FaceTowards(FreezeLook, DeltaSeconds);
		return;
	}

	FVector Target = Hero->GetActorLocation();
	float Stop = 30.f;
	switch (State)
	{
	case EState::Sent:
		Target = SentTarget;
		if (FVector::Dist2D(GetActorLocation(), SentTarget) < 60.f && FMath::Abs(GetActorLocation().Z - SentTarget.Z) < 100.f)
		{
			State = EState::Holding; // arrêt net, puis flaire
			HoldTime = 0.f;
			GetCharacterMovement()->StopMovementImmediately();
		}
		break;
	case EState::Holding:
	case EState::Staying:
		HoldTime += DeltaSeconds;
		return;
	case EState::Found:
		// Assise au bout de la piste, regard vers l'héroïne : « elle a trouvé quelque chose ».
		FaceTowards(Hero->GetActorLocation(), DeltaSeconds);
		return;
	case EState::Tracking:
	{
		Target = TrailPoints[TrailIndex];
		Stop = 40.f;
		if (FVector::Dist2D(GetActorLocation(), Target) < 80.f)
		{
			++TrailIndex;
			if (TrailIndex == TrailPoints.Num() / 2 && TrailPoints.Num() > 6)
			{
				// À mi-parcours, l'odeur se croise avec d'autres passages : cercles serrés.
				Signal(TEXT("UI_CARNET_CHIENNE_3"), TEXT("Cercles serrés : quelqu'un s'est arrêté ici, ou la piste se croise."));
			}
			if (TrailIndex >= TrailPoints.Num())
			{
				GetCharacterMovement()->StopMovementImmediately();
				if (bTrailSucceeds)
				{
					State = EState::Found;
					bSitting = true;
					SetMood(EFSDogMood::Talking, 6.f);
					Signal(TEXT("UI_CARNET_CHIENNE_4"), TEXT("Assise, regard vers moi : elle a trouvé quelque chose."));
				}
				else
				{
					Signal(TEXT("UI_CARNET_CHIENNE_5"), TEXT("Tête haute, retour vers moi : la piste s'arrête."));
					Recall();
				}
				return;
			}
		}
		break;
	}
	case EState::Talking:
	{
		// Vient s'asseoir face à l'héroïne, aboie, couine, frétille.
		TalkTime += DeltaSeconds;
		const FVector Front = Hero->GetActorLocation() + Hero->GetActorForwardVector() * 130.f;
		if (!bSitting && FVector::Dist2D(GetActorLocation(), Front) > 50.f && TalkTime < 8.f)
		{
			Target = Front;
			Stop = 40.f;
			break;
		}
		if (!bSitting)
		{
			bSitting = true;
			GetCharacterMovement()->StopMovementImmediately();
			Say(TEXT("DLG_P_ARIANE_PARLE_01"));
			TalkTime = 0.f;
		}
		FaceTowards(Hero->GetActorLocation(), DeltaSeconds);
		if (TalkTime > 3.f)
		{
			bSitting = false;
			if (bHasPointOfInterest)
			{
				State = EState::Leading;
				LeadStep = GetActorLocation();
				bWaitingForHero = true; // premier pas immédiat
			}
			else
			{
				Recall();
			}
		}
		return;
	}
	case EState::Leading:
	{
		// Quelques pas vers l'endroit, arrêt, se retourne et attend que l'héroïne suive.
		LeadTime += DeltaSeconds;
		const float HeroToDog = FVector::Dist2D(Hero->GetActorLocation(), GetActorLocation());
		const float DogToPoint = FVector::Dist2D(GetActorLocation(), PointOfInterest);
		if (LeadTime > 45.f || (DogToPoint < 300.f && HeroToDog < 600.f))
		{
			bHasPointOfInterest = false;
			State = EState::Holding;
			HoldTime = 0.f;
			SetMood(EFSDogMood::Alert, 5.f);
			return;
		}
		if (bWaitingForHero)
		{
			FaceTowards(Hero->GetActorLocation(), DeltaSeconds);
			if (HeroToDog < 450.f || LeadStep == GetActorLocation())
			{
				const FVector ToPoint = (PointOfInterest - GetActorLocation()).GetSafeNormal2D();
				LeadStep = GetActorLocation() + ToPoint * FMath::Min(600.f, DogToPoint);
				bWaitingForHero = false;
			}
			return;
		}
		Target = LeadStep;
		Stop = 40.f;
		if (FVector::Dist2D(GetActorLocation(), LeadStep) < 60.f)
		{
			bWaitingForHero = true;
			GetCharacterMovement()->StopMovementImmediately();
			if (!bLeadLineSaid)
			{
				bLeadLineSaid = true;
				if (AFSHUD* Hud = AFSHUD::Get(this))
				{
					Hud->PlayConversation({ FName(TEXT("DLG_P_ARIANE_PARLE_02")), FName(TEXT("DLG_P_ARIANE_PARLE_03")) });
				}
			}
			return;
		}
		break;
	}
	default:
	{
		// Libre à côté de l'héroïne, légèrement derrière et sur le côté ; entre elle et une menace si besoin.
		const FVector Fwd = Hero->GetActorForwardVector();
		const FVector Right = Hero->GetActorRightVector();
		Target = Hero->GetActorLocation() - Fwd * 80.f + Right * 90.f;
		if (ProtectFrom.IsValid())
		{
			const FVector ToThreat = (ProtectFrom->GetActorLocation() - Hero->GetActorLocation()).GetSafeNormal2D();
			Target = Hero->GetActorLocation() + ToThreat * 130.f;
		}
		Stop = FollowDistance * 0.4f;
		// Allure calée sur celle de l'héroïne à proximité (pas d'à-coups), trot puis sprint si elle est distancée.
		const float Dist = FVector::Dist2D(GetActorLocation(), Hero->GetActorLocation());
		const float HeroSpeed = Hero->GetVelocity().Size2D();
		const float Near = FMath::Max(HeroSpeed * 1.15f, Mood == EFSDogMood::Sad ? 110.f : 160.f);
		GetCharacterMovement()->MaxWalkSpeed = Dist > 600.f ? SprintSpeed
			: FMath::Lerp(Near, TrotSpeed, FMath::Clamp((Dist - 250.f) / 350.f, 0.f, 1.f));
		break;
	}
	}

	FVector ToTarget = Target - GetActorLocation();
	ToTarget.Z = 0.f;
	if (ToTarget.Size() > Stop)
	{
		AvoidObstacles(ToTarget);
		AddMovementInput(ToTarget.GetSafeNormal(), 1.f);
	}
	else if (bIsCrouched)
	{
		UnCrouch();
	}
}
