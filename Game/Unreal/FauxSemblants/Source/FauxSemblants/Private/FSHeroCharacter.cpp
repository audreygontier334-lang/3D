#include "FSHeroCharacter.h"
#include "FSPrologueDirector.h"
#include "FSDogCharacter.h"
#include "FSMissionSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
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
	Input->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	Input->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	Input->BindAction(TEXT("Run"), IE_Pressed, this, &AFSHeroCharacter::StartRun);
	Input->BindAction(TEXT("Run"), IE_Released, this, &AFSHeroCharacter::StopRun);
	Input->BindAction(TEXT("CameraShoulder"), IE_Pressed, this, &AFSHeroCharacter::CameraShoulder);
	Input->BindAction(TEXT("CameraWide"), IE_Pressed, this, &AFSHeroCharacter::CameraWide);
	Input->BindAction(TEXT("CameraFirst"), IE_Pressed, this, &AFSHeroCharacter::CameraFirst);
	Input->BindAction(TEXT("CameraCycle"), IE_Pressed, this, &AFSHeroCharacter::CameraCycle);
	Input->BindAction(TEXT("SwapShoulder"), IE_Pressed, this, &AFSHeroCharacter::SwapShoulder);
	Input->BindAction(TEXT("ActPhoto"), IE_Pressed, this, &AFSHeroCharacter::ActPhoto);
	Input->BindAction(TEXT("ActCrier"), IE_Pressed, this, &AFSHeroCharacter::ActCrier);
	Input->BindAction(TEXT("ActEnvoyer"), IE_Pressed, this, &AFSHeroCharacter::ActEnvoyer);
	Input->BindAction(TEXT("Rappel"), IE_Pressed, this, &AFSHeroCharacter::Rappel);
	Input->BindAction(TEXT("Appel17"), IE_Pressed, this, &AFSHeroCharacter::Appel17);
	Input->BindAction(TEXT("Save"), IE_Pressed, this, &AFSHeroCharacter::Save);
	SetCameraMode(EFSCameraMode::Shoulder);

	if (UMaterialInstanceDynamic* Mat = PlaceholderBody->CreateDynamicMaterialInstance(0))
	{
		Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.16f, 0.29f, 0.52f));
	}
}

void AFSHeroCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CameraArm->TargetArmLength = FMath::FInterpTo(CameraArm->TargetArmLength, TargetArmLength, DeltaSeconds, 8.f);
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
	if (GEngine)
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
	if (AFSPrologueDirector* D = FindDirector()) { D->TryAction(TEXT("ACT_PHOTO")); }
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
	if (AFSDogCharacter* Dog = FindDog()) { Dog->Recall(); }
}

void AFSHeroCharacter::Appel17()
{
	if (AFSPrologueDirector* D = FindDirector()) { D->CallPolice(); }
}

void AFSHeroCharacter::Save()
{
	if (UFSMissionSubsystem* M = GetGameInstance()->GetSubsystem<UFSMissionSubsystem>())
	{
		const bool bOk = M->SaveProgress();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(11, 2.f, FColor::White, bOk ? TEXT("Partie sauvegardée") : TEXT("Échec de la sauvegarde"));
		}
	}
}
