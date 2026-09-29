#include "FSMissionSubsystem.h"
#include "FauxSemblants.h"
#include "FSSaveGame.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/GameplayStatics.h"

TSharedPtr<FJsonObject> UFSMissionSubsystem::ReadJson(const FString& Path)
{
	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *Path))
	{
		UE_LOG(LogFauxSemblants, Error, TEXT("Fichier introuvable : %s (lancer Scripts/setup_prologue.py dans l'éditeur)"), *Path);
		return nullptr;
	}
	TSharedPtr<FJsonObject> Obj;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Text);
	if (!FJsonSerializer::Deserialize(Reader, Obj) || !Obj.IsValid())
	{
		UE_LOG(LogFauxSemblants, Error, TEXT("JSON illisible : %s"), *Path);
		return nullptr;
	}
	return Obj;
}

int32 UFSMissionSubsystem::ParseClockSeconds(const FString& Text)
{
	TArray<FString> Parts;
	Text.ParseIntoArray(Parts, TEXT(":"));
	if (Parts.Num() < 2)
	{
		return 0;
	}
	const int32 S = Parts.Num() > 2 ? FCString::Atoi(*Parts[2]) : 0;
	return FCString::Atoi(*Parts[0]) * 3600 + FCString::Atoi(*Parts[1]) * 60 + S;
}

bool UFSMissionSubsystem::LoadMission(const FString& InMissionFolder)
{
	MissionFolder = InMissionFolder;
	const FString Dir = FPaths::ProjectContentDir() / TEXT("Data") / MissionFolder;
	Mission = ReadJson(Dir / TEXT("mission.json"));
	const TSharedPtr<FJsonObject> CluesDoc = ReadJson(Dir / TEXT("clues.json"));
	const TSharedPtr<FJsonObject> DedDoc = ReadJson(Dir / TEXT("deductions.json"));
	const TSharedPtr<FJsonObject> DlgDoc = ReadJson(Dir / TEXT("dialogues.json"));
	if (!Mission.IsValid() || !CluesDoc.IsValid() || !DedDoc.IsValid() || !DlgDoc.IsValid())
	{
		return false;
	}

	Acquired.Reset();
	ClueFacts.Reset();
	Lines.Reset();
	Deductions.Reset();

	for (const TSharedPtr<FJsonValue>& V : CluesDoc->GetArrayField(TEXT("clues")))
	{
		const TSharedPtr<FJsonObject> C = V->AsObject();
		ClueFacts.Add(FName(*C->GetStringField(TEXT("id"))), C->GetStringField(TEXT("fact")));
	}
	for (const TSharedPtr<FJsonValue>& V : DedDoc->GetArrayField(TEXT("deductions")))
	{
		const TSharedPtr<FJsonObject> D = V->AsObject();
		TArray<TArray<FName>>& Sets = Deductions.Add(FName(*D->GetStringField(TEXT("id"))));
		for (const TSharedPtr<FJsonValue>& SetValue : D->GetArrayField(TEXT("any_of")))
		{
			TArray<FName>& Ids = Sets.AddDefaulted_GetRef();
			for (const TSharedPtr<FJsonValue>& Id : SetValue->AsArray())
			{
				Ids.Add(FName(*Id->AsString()));
			}
		}
	}
	for (const TSharedPtr<FJsonValue>& SceneValue : DlgDoc->GetArrayField(TEXT("scenes")))
	{
		for (const TSharedPtr<FJsonValue>& LineValue : SceneValue->AsObject()->GetArrayField(TEXT("lines")))
		{
			const TSharedPtr<FJsonObject> L = LineValue->AsObject();
			Lines.Add(FName(*L->GetStringField(TEXT("id"))), L->GetStringField(TEXT("text")));
		}
	}

	const TSharedPtr<FJsonObject>* Clock = nullptr;
	if (Mission->TryGetObjectField(TEXT("clock"), Clock))
	{
		FString Start;
		if ((*Clock)->TryGetStringField(TEXT("prologue_start"), Start) || (*Clock)->TryGetStringField(TEXT("chapter_start"), Start))
		{
			ClockMinutes = ParseClockSeconds(Start) / 60;
		}
	}
	UE_LOG(LogFauxSemblants, Log, TEXT("Mission %s chargée : %d indices, %d déductions, %d répliques"),
		*MissionFolder, ClueFacts.Num(), Deductions.Num(), Lines.Num());
	return true;
}

void UFSMissionSubsystem::Grant(FName Id)
{
	if (Id.IsNone() || Acquired.Contains(Id))
	{
		return;
	}
	Acquired.Add(Id);
	OnAcquired.Broadcast(Id);
	DeriveDeductions();
}

void UFSMissionSubsystem::DeriveDeductions()
{
	bool bChanged = true;
	while (bChanged)
	{
		bChanged = false;
		for (const TPair<FName, TArray<TArray<FName>>>& Pair : Deductions)
		{
			if (Acquired.Contains(Pair.Key))
			{
				continue;
			}
			for (const TArray<FName>& Set : Pair.Value)
			{
				bool bAll = Set.Num() > 0;
				for (const FName& Id : Set)
				{
					bAll &= Acquired.Contains(Id);
				}
				if (bAll)
				{
					Acquired.Add(Pair.Key);
					OnAcquired.Broadcast(Pair.Key);
					bChanged = true;
					break;
				}
			}
		}
	}
}

FString UFSMissionSubsystem::GetLineText(FName LineId) const
{
	const FString* Text = Lines.Find(LineId);
	return Text ? Text->Replace(TEXT("{HEROINE}"), *HeroineName) : FString();
}

FString UFSMissionSubsystem::GetClueFact(FName ClueId) const
{
	const FString* Text = ClueFacts.Find(ClueId);
	return Text ? Text->Replace(TEXT("{HEROINE}"), *HeroineName) : FString();
}

TSharedPtr<FJsonObject> UFSMissionSubsystem::GetPrologue() const
{
	const TSharedPtr<FJsonObject>* Prologue = nullptr;
	return (Mission.IsValid() && Mission->TryGetObjectField(TEXT("prologue"), Prologue)) ? *Prologue : nullptr;
}

bool UFSMissionSubsystem::SaveProgress(const FString& SlotName)
{
	UFSSaveGame* Save = Cast<UFSSaveGame>(UGameplayStatics::CreateSaveGameObject(UFSSaveGame::StaticClass()));
	Save->MissionFolder = MissionFolder;
	Save->ClockMinutes = ClockMinutes;
	Save->DataVersion = DataVersion;
	Save->Acquired = Acquired.Array();
	return UGameplayStatics::SaveGameToSlot(Save, SlotName, 0);
}

bool UFSMissionSubsystem::LoadProgress(const FString& SlotName)
{
	const UFSSaveGame* Save = Cast<UFSSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!Save || Save->DataVersion != DataVersion || !LoadMission(Save->MissionFolder))
	{
		return false;
	}
	ClockMinutes = Save->ClockMinutes;
	Acquired = TSet<FName>(Save->Acquired);
	DeriveDeductions();
	return true;
}
