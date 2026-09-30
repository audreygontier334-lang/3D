// Faux-semblants — l'héroïne jouable : déplacement, course, trois vues (épaule / reculée / subjective).
// Changer de vue ne modifie ni l'heure, ni les indices, ni l'état d'Ariane (docs/DIRECTION_VISUELLE.md).
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class AFSPrologueDirector;
class AFSDogCharacter;

UENUM(BlueprintType)
enum class EFSCameraMode : uint8
{
	Shoulder UMETA(DisplayName = "Épaule"),
	Wide UMETA(DisplayName = "Reculée"),
	First UMETA(DisplayName = "Subjective")
};

UCLASS()
class FAUXSEMBLANTS_API AFSHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFSHeroCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void SetCameraMode(EFSCameraMode NewMode);

	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	EFSCameraMode GetCameraMode() const { return CameraMode; }

	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	bool IsRunning() const { return bRunning; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UCameraComponent> Camera;

	/** Volume provisoire (pas de modèle photoréaliste avant validation d'Audrey). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UStaticMeshComponent> PlaceholderBody;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float WalkSpeed = 170.f;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float RunSpeed = 500.f;

	/** Vitesse de rotation de la caméra au stick droit, en degrés par seconde. */
	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float GamepadTurnRate = 100.f;

	/** Invite affichée par l'interface (« E : saluer Marcel Dufau »…), vide s'il n'y a rien à faire. */
	FString GetInteractionPrompt() const;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartRun();
	void StopRun();
	void CameraShoulder() { SetCameraMode(EFSCameraMode::Shoulder); }
	void CameraWide() { SetCameraMode(EFSCameraMode::Wide); }
	void CameraFirst() { SetCameraMode(EFSCameraMode::First); }
	void CameraCycle();
	void SwapShoulder();
	void ActPhoto();
	void ActCrier();
	void ActEnvoyer();
	void Rappel();
	void Appel17();
	void Save();
	void PressStart();
	void ToggleNotebook();
	void Turn(float Value);
	void LookUp(float Value);
	void TurnRate(float Value);
	void LookUpRate(float Value);
	void Zoom(float Value);
	void Interact();
	void Reste();
	void Cherche();
	void TogglePause();
	void MenuUp();
	void MenuDown();
	void MenuLeft();
	void MenuRight();

	/** Personne ou action à portée : 0 rien, 1 coucou de Lila, 2 Dufau. */
	int32 FindInteraction() const;

	AFSPrologueDirector* FindDirector() const;
	AFSDogCharacter* FindDog() const;

	EFSCameraMode CameraMode = EFSCameraMode::Shoulder;
	bool bRunning = false;
	float ShoulderSide = 1.f;
	float TargetArmLength = 240.f;
	FVector TargetSocketOffset = FVector::ZeroVector;
	float ZoomOffset = 0.f;
	bool bTalkedToDufau = false;
	bool bBallFoundSaid = false;
	UPROPERTY() TObjectPtr<AActor> Ball;
};
