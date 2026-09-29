// Faux-semblants — Ariane : libre (ni laisse ni collier, décision d'Audrey), suit l'héroïne à 1–3 m,
// revient au rappel, et, envoyée, fonce vers un point puis s'arrête net (bord de chaussée).
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSDogCharacter.generated.h"

class UStaticMeshComponent;

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

	UFUNCTION(BlueprintCallable, Category = "FauxSemblants")
	void Recall();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FauxSemblants")
	TObjectPtr<UStaticMeshComponent> PlaceholderBody;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float FollowDistance = 180.f;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float TrotSpeed = 520.f;

	UPROPERTY(EditAnywhere, Category = "FauxSemblants")
	float SprintSpeed = 850.f;

private:
	enum class EState : uint8 { Follow, Sent, Holding };
	EState State = EState::Follow;
	FVector SentTarget = FVector::ZeroVector;
	float HoldTime = 0.f;
};
