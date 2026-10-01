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
class USpotLightComponent;
class UFSSaveGame;

/** Objet du sac en bandoulière de l'héroïne. */
struct FFSBagItem
{
	const TCHAR* Id;
	const TCHAR* Name;
	const TCHAR* Description;
};

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

	/** Lampe torche fixée à la caméra (inventaire ou touche L). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<USpotLightComponent> Torch;

	// --- Inventaire ---------------------------------------------------------------------------
	static const TArray<FFSBagItem>& BagItems();
	/** Texte d'état d'un objet du sac (« 12 restants », « portés »…). */
	FString BagItemState(const FString& Id) const;
	/** Utilise un objet du sac ; renvoie le message à afficher. */
	FString UseBagItem(const FString& Id);
	/** Examine ou utilise une preuve (CLU_…) de l'inventaire. */
	FString UseEvidence(FName ClueId);
	bool AreGlovesOn() const { return bGloves; }
	bool IsTorchOn() const;
	void SetTorch(bool bOn);
	void ToggleTorch() { SetTorch(!IsTorchOn()); }

	/** F : photo (ACT_PHOTO pendant l'alerte, sinon simple photo pour la galerie). */
	void ActPhoto();

	/** Photo avec le téléphone : décrit ce qui est dans le cadre et l'ajoute à la galerie ; renvoie la description. */
	FString TakePhoto();

	/** Fait sentir une preuve (objet) à Ariane, qui suit la piste correspondante si elle existe. */
	FString SniffEvidence(FName ClueId);

	/** Sauvegarde et reprise (menu principal › Sauvegardes). */
	void WriteState(UFSSaveGame& Save) const;
	void ReadState(const UFSSaveGame& Save);

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
	void ActCrier();
	void ActEnvoyer();
	void Rappel();
	void Appel17();
	void Save();
	void ToggleNotebook();
	void OpenInventory();
	void OpenPhone();
	void QuickLoad();
	void Turn(float Value);
	void LookUp(float Value);
	void TurnRate(float Value);
	void LookUpRate(float Value);
	void Zoom(float Value);
	void Interact();
	void Reste();
	void Cherche();
	void Non();
	void Bravo();
	void Montre();
	void VaLaBas();
	void RecordSockTrail();
	FString DescribeView(bool& bVanVisible) const;
	void TogglePause();

	/** Personne ou action à portée : 0 rien, 1 coucou de Lila, 2 Dufau, 3 preuve à ramasser, 4 ficelle ou corde à couper,
	 *  5 chaussette à reprendre (PickupTarget = acteur concerné). */
	int32 FindInteraction(AActor** PickupTarget = nullptr) const;

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
	int32 Candies = 12;
	bool bGloves = false;
	TArray<FName> PickedUp;

	// Exercice de pistage : chaussette posée et chemin parcouru depuis.
	UPROPERTY() TObjectPtr<AActor> Sock;
	TArray<FVector> SockTrail;
	float TorchCheck = 0.f;
};
