// Faux-semblants — téléphone de l'héroïne : répertoire, messages, mails, notifications, journal d'appels.
// Contenus : GameData/telephone/01-prologue.json (copié dans Content/Data/<mission>/telephone.json).
// Les éléments sont délivrés par les événements du prologue (Trigger) ; l'état se sauvegarde avec la partie.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FSPhoneSubsystem.generated.h"

USTRUCT()
struct FFSPhoneContact
{
	GENERATED_BODY()
	UPROPERTY() FName Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Number;
	UPROPERTY() FString Voicemail;
	UPROPERTY() bool bEmergency = false;
};

USTRUCT()
struct FFSPhoneItem
{
	GENERATED_BODY()
	UPROPERTY() FName Id;
	UPROPERTY() FString Kind;     // message, mail, notification, call
	UPROPERTY() FString From;     // contact, expéditeur ou application
	UPROPERTY() FString Title;    // objet du mail, sens de l'appel
	UPROPERTY() FString Body;
	UPROPERTY() FString At;
	UPROPERTY() FString Trigger;
	UPROPERTY() bool bDelivered = false;
	UPROPERTY() bool bRead = false;
	UPROPERTY() TArray<FString> Replies;
	UPROPERTY() bool bReplied = false;
};

UCLASS()
class FAUXSEMBLANTS_API UFSPhoneSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Charge Content/Data/<MissionFolder>/telephone.json et délivre les éléments « START ». */
	bool LoadPhone(const FString& MissionFolder, const FString& HeroineName);

	/** Délivre les éléments liés à cet événement (EVT_…) ; renvoie le nombre de nouveautés. */
	int32 Trigger(const FString& EventId);

	/** Résultat d'un appel : texte à afficher ; bEmergency vrai pour le 17 ou le 112. Ajoute l'appel au journal. */
	FString Call(const FString& Number, const FString& ClockText, bool& bEmergency);

	const TArray<FFSPhoneContact>& GetContacts() const { return Contacts; }
	TArray<FFSPhoneItem*> GetItems(const FString& Kind);
	int32 CountUnread(const FString& Kind) const;
	void MarkRead(FName Id);
	FString ContactName(const FString& NumberOrId) const;
	FString OwnerNumber() const { return Owner; }
	FString CallRule(const TCHAR* Key) const;

	/** Appareil photo : ajoute une photo à la galerie (« photo »). */
	void AddPhoto(const FString& At, const FString& Description);

	/** Envoie la réponse ReplyIndex au message MessageId ; renvoie le texte envoyé. */
	FString Reply(FName MessageId, int32 ReplyIndex, const FString& At);

	/** Sauvegarde : IDs délivrés, IDs lus, appels passés pendant la partie (« heure|numéro|sens »),
	 *  éléments ajoutés pendant la partie (photos, réponses : « type|heure|de|titre|texte »). */
	void GetState(TArray<FName>& Delivered, TArray<FName>& Read, TArray<FString>& PlacedCalls, TArray<FString>& Extras) const;
	void SetState(const TArray<FName>& Delivered, const TArray<FName>& Read, const TArray<FString>& PlacedCalls, const TArray<FString>& Extras);

private:
	FString Normalize(const FString& Number) const;
	void AddPlacedCall(const FString& Entry);
	void AddExtra(const FString& Entry);

	TArray<FFSPhoneContact> Contacts;
	TArray<FFSPhoneItem> Items;
	TMap<FString, FString> Rules;
	TArray<FString> PlacedCalls;
	TArray<FString> Extras;
	FString Owner;
	FString Heroine;
	FString LoadedFolder;
};
