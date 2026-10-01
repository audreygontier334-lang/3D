// Faux-semblants — Ariane : libre (ni laisse ni collier, décision d'Audrey), suit l'héroïne à son rythme,
// revient toujours au rappel, et a sa propre personnalité (comportements décrits par Audrey le 01/10/2026) :
//  - expressions des oreilles et de la queue : à l'affût (tout dressé), triste ou grondée (oreilles couchées en
//    arrière, queue basse), contente (queue qui remue, sautille) ; au repos, une oreille droite et l'autre tombante ;
//  - elle vient « parler » (s'assoit face à l'héroïne, aboie, couine, frétille) puis montre ou emmène vers un endroit ;
//  - elle part comme une flèche sur une piste, grogne quand quelque chose est anormal (parfois un hérisson),
//    s'interpose entre l'héroïne ou un enfant et une menace ;
//  - elle rampe sous les obstacles bas, saute très haut (plus de 2 m) et ouvre les portes (acteurs taggés « Porte »).
// Les volumes provisoires (corps, oreilles, queue) montrent déjà ces états ; Codex branchera ses animations sur GetMood().
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSDogCharacter.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EFSDogMood : uint8
{
	Neutral UMETA(DisplayName = "Au repos"),
	Alert UMETA(DisplayName = "À l'affût"),
	Happy UMETA(DisplayName = "Contente"),
	Sad UMETA(DisplayName = "Triste"),
	Talking UMETA(DisplayName = "Veut parler")
};

UCLASS()
class FAUXSEMBLANTS_API AFSDogCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFSDogCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Envoie Ariane vers un point (monde) ; elle s'y arrête et flaire. */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void SendTo(const FVector& Target);

	/** « Au pied » : elle revient toujours. */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Recall();

	/** « Reste » : elle s'arrête et attend « Au pied » ou « Va ». */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Stay();

	/** « Non ! » : elle s'arrête, revient penaude (oreilles couchées, queue basse). */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Scold();

	/** « C'est bien ! » : contente, elle remue la queue et sautille. */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Praise();

	/** « Montre » : si elle veut montrer quelque chose, elle y emmène l'héroïne ; sinon elle penche la tête. */
	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Show();

	/** Elle vient « parler » puis emmène l'héroïne vers Point, par petites étapes, en se retournant pour l'attendre. */
	void LeadTo(const FVector& Point);

	/** Elle part comme une flèche sur une piste jusqu'à Point. */
	void Bolt(const FVector& Point);

	/** Endroit qu'elle aimerait montrer (utilisé par « Montre »). */
	void SetPointOfInterest(const FVector& Point) { PointOfInterest = Point; bHasPointOfInterest = true; }

	/** Pistage : elle suit les points dans l'ordre, truffe au sol ; au bout, elle s'assoit et regarde l'héroïne
	 *  (bFoundAtEnd, langage « elle a trouvé quelque chose ») ou relève la tête et revient (« la piste s'arrête »). */
	void Track(const TArray<FVector>& Points, bool bFoundAtEnd);

	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	bool IsTracking() const { return State == EState::Tracking; }

	/** Vrai quand elle est assise au bout d'une piste réussie, près de Point. */
	bool HasFoundNear(const FVector& Point, float Radius = 200.f) const;

	void SetMood(EFSDogMood NewMood, float Seconds);

	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	EFSDogMood GetMood() const { return Mood; }

	UFUNCTION(BlueprintPure, Category = "FauxSemblants")
	bool IsSitting() const { return bSitting; }

	/** Vrai si Ariane, envoyée, est arrivée et flaire à moins de Radius cm de Point. */
	bool IsHoldingNear(const FVector& Point, float Radius = 120.f) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<USceneComponent> Visual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UStaticMeshComponent> PlaceholderBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UStaticMeshComponent> EarRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UStaticMeshComponent> EarLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UStaticMeshComponent> Tail;

	/** Pivots : base des oreilles et racine de la queue (les animations de Codex pourront s'y substituer). */
	UPROPERTY(VisibleAnywhere, Category = "FauxSemblants")
	TObjectPtr<USceneComponent> EarRightPivot;

	UPROPERTY(VisibleAnywhere, Category = "FauxSemblants")
	TObjectPtr<USceneComponent> EarLeftPivot;

	UPROPERTY(VisibleAnywhere, Category = "FauxSemblants")
	TObjectPtr<USceneComponent> TailPivot;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float FollowDistance = 180.f;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float TrotSpeed = 520.f;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float SprintSpeed = 850.f;

private:
	enum class EState : uint8 { Follow, Sent, Holding, Staying, Talking, Leading, Tracking, Found };

	void Say(const TCHAR* LineId);
	void Signal(const TCHAR* UIId, const TCHAR* Fallback);
	void Sense(float DeltaSeconds);
	void AvoidObstacles(const FVector& Direction);
	void AnimateExpressions(float DeltaSeconds);
	void FaceTowards(const FVector& Point, float DeltaSeconds);
	bool IsThreat(const AActor* Actor) const;

	EState State = EState::Follow;
	FVector SentTarget = FVector::ZeroVector;
	float HoldTime = 0.f;

	EFSDogMood Mood = EFSDogMood::Neutral;
	float MoodLeft = 0.f;
	bool bSitting = false;
	float TiltLeft = 0.f;

	// « Parler » et « emmener vers ».
	FVector PointOfInterest = FVector::ZeroVector;
	bool bHasPointOfInterest = false;
	FVector LeadStep = FVector::ZeroVector;
	float TalkTime = 0.f;
	float LeadTime = 0.f;
	bool bWaitingForHero = false;
	bool bLeadLineSaid = false;

	// Sens et réactions.
	float SenseTimer = 0.f;
	float GrowlCooldown = 0.f;
	float ProtectCooldown = 0.f;
	bool bHedgehogGrowled = false;
	bool bHedgehogExplained = false;
	float FreezeLeft = 0.f;
	FVector FreezeLook = FVector::ZeroVector;
	TWeakObjectPtr<AActor> ProtectFrom;
	float JumpLineCooldown = 0.f;
	bool bDoorLineSaid = false;

	// Pistage.
	TArray<FVector> TrailPoints;
	int32 TrailIndex = 0;
	bool bTrailSucceeds = false;
	float TrailEndTime = 0.f;

	// Expressions (angles courants, lissés).
	float EarRightPitch = 0.f, EarLeftPitch = 60.f, EarLeftRoll = 50.f, TailPitch = 20.f, TailYaw = 0.f, BodyPitch = 0.f, Bounce = 0.f;
	float Clock = 0.f;
};
