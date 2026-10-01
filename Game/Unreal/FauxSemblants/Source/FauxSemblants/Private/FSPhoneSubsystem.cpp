#include "FSPhoneSubsystem.h"
#include "FauxSemblants.h"
#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

bool UFSPhoneSubsystem::LoadPhone(const FString& MissionFolder, const FString& HeroineName)
{
	Heroine = HeroineName;
	Contacts.Reset();
	Items.Reset();
	Rules.Reset();
	PlacedCalls.Reset();
	Extras.Reset();
	LoadedFolder = MissionFolder;

	FString Text;
	const FString Path = FPaths::ProjectContentDir() / TEXT("Data") / MissionFolder / TEXT("telephone.json");
	TSharedPtr<FJsonObject> Doc;
	if (!FFileHelper::LoadFileToString(Text, *Path) || !FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), Doc) || !Doc.IsValid())
	{
		UE_LOG(LogFauxSemblants, Warning, TEXT("Téléphone : %s illisible (relancer Scripts/setup_prologue.py)"), *Path);
		return false;
	}
	Doc->TryGetStringField(TEXT("owner_number"), Owner);

	const TArray<TSharedPtr<FJsonValue>>* Array = nullptr;
	if (Doc->TryGetArrayField(TEXT("contacts"), Array))
	{
		for (const TSharedPtr<FJsonValue>& V : *Array)
		{
			const TSharedPtr<FJsonObject> O = V->AsObject();
			FFSPhoneContact& C = Contacts.AddDefaulted_GetRef();
			C.Id = FName(*O->GetStringField(TEXT("id")));
			C.Name = O->GetStringField(TEXT("name"));
			C.Number = O->GetStringField(TEXT("number"));
			O->TryGetStringField(TEXT("voicemail"), C.Voicemail);
			O->TryGetBoolField(TEXT("emergency"), C.bEmergency);
		}
	}
	auto ReadItems = [&](const TCHAR* Field, const TCHAR* Kind)
	{
		const TArray<TSharedPtr<FJsonValue>>* List = nullptr;
		if (!Doc->TryGetArrayField(Field, List)) { return; }
		for (const TSharedPtr<FJsonValue>& V : *List)
		{
			const TSharedPtr<FJsonObject> O = V->AsObject();
			FFSPhoneItem& I = Items.AddDefaulted_GetRef();
			I.Id = FName(*O->GetStringField(TEXT("id")));
			I.Kind = Kind;
			O->TryGetStringField(TEXT("at"), I.At);
			O->TryGetStringField(TEXT("trigger"), I.Trigger);
			FString From;
			if (O->TryGetStringField(TEXT("from"), From)) { I.From = ContactName(From); }
			if (O->TryGetStringField(TEXT("app"), From)) { I.From = From; }
			if (O->TryGetStringField(TEXT("number"), From)) { I.From = ContactName(From); I.Body = From; }
			O->TryGetStringField(TEXT("subject"), I.Title);
			O->TryGetStringField(TEXT("direction"), I.Title);
			FString Body;
			if (O->TryGetStringField(TEXT("text"), Body) || O->TryGetStringField(TEXT("body"), Body)) { I.Body = Body; }
			I.Body.ReplaceInline(TEXT("{HEROINE}"), *Heroine);
			const TArray<TSharedPtr<FJsonValue>>* RepliesJson = nullptr;
			if (O->TryGetArrayField(TEXT("replies"), RepliesJson))
			{
				for (const TSharedPtr<FJsonValue>& R : *RepliesJson) { I.Replies.Add(R->AsString()); }
			}
			I.bRead = I.Trigger == TEXT("START") && I.Kind != TEXT("message");
		}
	};
	ReadItems(TEXT("messages"), TEXT("message"));
	ReadItems(TEXT("mails"), TEXT("mail"));
	ReadItems(TEXT("notifications"), TEXT("notification"));
	ReadItems(TEXT("calls"), TEXT("call"));

	const TSharedPtr<FJsonObject>* RulesObj = nullptr;
	if (Doc->TryGetObjectField(TEXT("call_rules"), RulesObj))
	{
		for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : (*RulesObj)->Values)
		{
			if (!Pair.Key.StartsWith(TEXT("_"))) { Rules.Add(Pair.Key, Pair.Value->AsString()); }
		}
	}
	Trigger(TEXT("START"));
	return true;
}

int32 UFSPhoneSubsystem::Trigger(const FString& EventId)
{
	int32 Count = 0;
	for (FFSPhoneItem& I : Items)
	{
		if (!I.bDelivered && I.Trigger == EventId)
		{
			I.bDelivered = true;
			++Count;
		}
	}
	return EventId == TEXT("START") ? 0 : Count;
}

TArray<FFSPhoneItem*> UFSPhoneSubsystem::GetItems(const FString& Kind)
{
	TArray<FFSPhoneItem*> Out;
	// Du plus récent au plus ancien : les éléments sont listés dans l'ordre d'arrivée du fichier.
	for (int32 i = Items.Num() - 1; i >= 0; --i)
	{
		if (Items[i].bDelivered && Items[i].Kind == Kind) { Out.Add(&Items[i]); }
	}
	return Out;
}

int32 UFSPhoneSubsystem::CountUnread(const FString& Kind) const
{
	int32 N = 0;
	for (const FFSPhoneItem& I : Items)
	{
		if (I.bDelivered && !I.bRead && I.Kind == Kind) { ++N; }
	}
	return N;
}

void UFSPhoneSubsystem::MarkRead(FName Id)
{
	for (FFSPhoneItem& I : Items)
	{
		if (I.Id == Id) { I.bRead = true; }
	}
}

FString UFSPhoneSubsystem::Normalize(const FString& Number) const
{
	FString Out;
	for (const TCHAR C : Number)
	{
		if (FChar::IsDigit(C)) { Out.AppendChar(C); }
	}
	return Out;
}

FString UFSPhoneSubsystem::ContactName(const FString& NumberOrId) const
{
	const FString N = Normalize(NumberOrId);
	for (const FFSPhoneContact& C : Contacts)
	{
		if (C.Id.ToString() == NumberOrId || (!N.IsEmpty() && Normalize(C.Number) == N)) { return C.Name; }
	}
	return NumberOrId;
}

FString UFSPhoneSubsystem::CallRule(const TCHAR* Key) const
{
	const FString* Rule = Rules.Find(Key);
	return Rule ? *Rule : FString();
}

void UFSPhoneSubsystem::AddPlacedCall(const FString& Entry)
{
	PlacedCalls.Add(Entry);
	FFSPhoneItem& I = Items.AddDefaulted_GetRef();
	TArray<FString> Parts;
	Entry.ParseIntoArray(Parts, TEXT("|"), false);
	I.Id = FName(*FString::Printf(TEXT("CALL_PARTIE_%d"), PlacedCalls.Num()));
	I.Kind = TEXT("call");
	I.At = Parts.IsValidIndex(0) ? Parts[0] : FString();
	I.Body = Parts.IsValidIndex(1) ? Parts[1] : FString();
	I.From = ContactName(I.Body);
	I.Title = Parts.IsValidIndex(2) ? Parts[2] : TEXT("sortant");
	I.bDelivered = true;
	I.bRead = true;
}

FString UFSPhoneSubsystem::Call(const FString& Number, const FString& ClockText, bool& bEmergency)
{
	const FString N = Normalize(Number);
	bEmergency = N == TEXT("17") || N == TEXT("112");
	AddPlacedCall(FString::Printf(TEXT("%s|%s|sortant"), *ClockText, *Number));
	if (bEmergency)
	{
		return FString(); // la suite dépend du moment du prologue : décidée par l'interface et le directeur
	}
	for (const FFSPhoneContact& C : Contacts)
	{
		if (!N.IsEmpty() && Normalize(C.Number) == N)
		{
			return C.Voicemail.IsEmpty() ? FString::Printf(TEXT("%s ne répond pas."), *C.Name) : C.Voicemail;
		}
	}
	return CallRule(TEXT("unknown_number"));
}

void UFSPhoneSubsystem::AddExtra(const FString& Entry)
{
	Extras.Add(Entry);
	TArray<FString> Parts;
	Entry.ParseIntoArray(Parts, TEXT("|"), false);
	FFSPhoneItem& I = Items.AddDefaulted_GetRef();
	I.Id = FName(*FString::Printf(TEXT("EXTRA_%d"), Extras.Num()));
	I.Kind = Parts.IsValidIndex(0) ? Parts[0] : TEXT("photo");
	I.At = Parts.IsValidIndex(1) ? Parts[1] : FString();
	I.From = Parts.IsValidIndex(2) ? Parts[2] : FString();
	I.Title = Parts.IsValidIndex(3) ? Parts[3] : FString();
	I.Body = Parts.IsValidIndex(4) ? Parts[4] : FString();
	I.bDelivered = true;
	I.bRead = true;
	if (Parts.IsValidIndex(5))
	{
		// Réponse à un message : le message d'origine n'accepte plus d'autre réponse.
		for (FFSPhoneItem& Other : Items) { if (Other.Id == FName(*Parts[5])) { Other.bReplied = true; } }
	}
}

void UFSPhoneSubsystem::AddPhoto(const FString& At, const FString& Description)
{
	AddExtra(FString::Printf(TEXT("photo|%s|Appareil photo|Photo|%s"), *At, *Description.Replace(TEXT("|"), TEXT("/"))));
}

FString UFSPhoneSubsystem::Reply(FName MessageId, int32 ReplyIndex, const FString& At)
{
	for (const FFSPhoneItem& I : Items)
	{
		if (I.Id == MessageId && !I.bReplied && I.Replies.IsValidIndex(ReplyIndex))
		{
			const FString Text = I.Replies[ReplyIndex];
			AddExtra(FString::Printf(TEXT("message|%s|Moi|%s|%s|%s"), *At, *I.From, *Text.Replace(TEXT("|"), TEXT("/")), *MessageId.ToString()));
			return Text;
		}
	}
	return FString();
}

void UFSPhoneSubsystem::GetState(TArray<FName>& Delivered, TArray<FName>& Read, TArray<FString>& OutCalls, TArray<FString>& OutExtras) const
{
	OutExtras = Extras;
	for (const FFSPhoneItem& I : Items)
	{
		if (I.Id.ToString().StartsWith(TEXT("CALL_PARTIE_")) || I.Id.ToString().StartsWith(TEXT("EXTRA_"))) { continue; }
		if (I.bDelivered) { Delivered.Add(I.Id); }
		if (I.bRead) { Read.Add(I.Id); }
	}
	OutCalls = PlacedCalls;
}

void UFSPhoneSubsystem::SetState(const TArray<FName>& Delivered, const TArray<FName>& Read, const TArray<FString>& InCalls, const TArray<FString>& InExtras)
{
	LoadPhone(LoadedFolder, Heroine);
	for (FFSPhoneItem& I : Items)
	{
		I.bDelivered = Delivered.Contains(I.Id);
		I.bRead = Read.Contains(I.Id);
	}
	for (const FString& Entry : InCalls)
	{
		AddPlacedCall(Entry);
	}
	for (const FString& Entry : InExtras)
	{
		AddExtra(Entry);
	}
}
