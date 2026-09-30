#include "FSDogCharacter.h"
#include "FSPrologueRules.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AFSDogCharacter::AFSDogCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(28.f, 32.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = TrotSpeed;
	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PlaceholderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderBody"));
	PlaceholderBody->SetupAttachment(GetCapsuleComponent());
	PlaceholderBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		PlaceholderBody->SetStaticMesh(Cube.Object);
		PlaceholderBody->SetRelativeScale3D(FVector(0.95f, 0.36f, 0.6f)); // ≈ 35 kg, 60 cm au garrot
		PlaceholderBody->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	}
}

void AFSDogCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UMaterialInstanceDynamic* Mat = PlaceholderBody->CreateDynamicMaterialInstance(0))
	{
		Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.62f, 0.44f, 0.25f)); // beige fauve
	}
}

void AFSDogCharacter::SendTo(const FVector& Target)
{
	++SendRequestId;
	SentTarget = Target;
	State = EState::Sent;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFSDogCharacter::Recall()
{
	++SendRequestId;
	State = EState::Follow;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->MaxWalkSpeed = TrotSpeed;
}

bool AFSDogCharacter::HasReachedSendTarget(uint32 RequestId) const
{
	return FSPrologueRules::HasReachedTarget(State == EState::Holding, SendRequestId, RequestId,
		FVector::Dist2D(GetActorLocation(), SentTarget), FMath::Abs(GetActorLocation().Z - SentTarget.Z));
}

void AFSDogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const APawn* Hero = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Hero)
	{
		return;
	}

	FVector Target;
	float Stop = 30.f;
	if (State == EState::Sent)
	{
		Target = SentTarget;
		if (FVector::Dist2D(GetActorLocation(), SentTarget) < 60.f
			&& FMath::Abs(GetActorLocation().Z - SentTarget.Z) < 100.f)
		{
			State = EState::Holding; // arrêt net, puis flaire
			HoldTime = 0.f;
			GetCharacterMovement()->StopMovementImmediately();
		}
	}
	if (State == EState::Holding)
	{
		HoldTime += DeltaSeconds;
		return;
	}
	if (State == EState::Follow)
	{
		// Libre à côté de l'héroïne, légèrement derrière et sur le côté.
		const FVector Fwd = Hero->GetActorForwardVector();
		const FVector Right = Hero->GetActorRightVector();
		Target = Hero->GetActorLocation() - Fwd * 80.f + Right * 90.f;
		Stop = FollowDistance * 0.4f;
		const float Dist = FVector::Dist2D(GetActorLocation(), Hero->GetActorLocation());
		GetCharacterMovement()->MaxWalkSpeed = Dist > 600.f ? SprintSpeed : TrotSpeed;
	}

	FVector ToTarget = Target - GetActorLocation();
	ToTarget.Z = 0.f;
	if (ToTarget.Size() > Stop)
	{
		AddMovementInput(ToTarget.GetSafeNormal(), 1.f);
	}
}
