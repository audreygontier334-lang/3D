#include "FSSaveGame.h"
#include "FauxSemblants.h"
#include "FSDogCharacter.h"
#include "FSHeroCharacter.h"
#include "FSMissionSubsystem.h"
#include "FSPhoneSubsystem.h"
#include "FSPrologueDirector.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	constexpr int32 SaveVersion = 2;
}

FString UFSSaveGame::SlotName(int32 Index)
{
	return Index <= 0 ? FString(TEXT("FauxSemblants_Rapide")) : FString::Printf(TEXT("FauxSemblants_%d"), Index);
}

bool UFSSaveGame::SaveSlot(const UObject* WorldContext, int32 Index)
{
	const UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
	if (!World)
	{
		return false;
	}
	UFSSaveGame* Save = Cast<UFSSaveGame>(UGameplayStatics::CreateSaveGameObject(UFSSaveGame::StaticClass()));
	UGameInstance* GI = World->GetGameInstance();
	UFSMissionSubsystem* Mission = GI ? GI->GetSubsystem<UFSMissionSubsystem>() : nullptr;
	if (!Save || !Mission)
	{
		return false;
	}
	Save->DataVersion = SaveVersion;
	Save->MissionFolder = TEXT("M01");
	Save->ClockMinutes = Mission->GetClockMinutes();
	Save->Acquired = Mission->GetAcquiredOrder();
	Save->SavedAt = FDateTime::Now();

	if (AFSPrologueDirector* D = Cast<AFSPrologueDirector>(UGameplayStatics::GetActorOfClass(World, AFSPrologueDirector::StaticClass())))
	{
		D->WriteState(*Save);
		const int32 Secs = FMath::FloorToInt(Save->PrologueSeconds);
		Save->Summary = FString::Printf(TEXT("Mardi %02d:%02d"), 16 + (25 + Secs / 60) / 60, (25 + Secs / 60) % 60);
	}
	if (AFSHeroCharacter* Hero = Cast<AFSHeroCharacter>(UGameplayStatics::GetPlayerPawn(World, 0)))
	{
		Hero->WriteState(*Save);
	}
	for (TActorIterator<AFSDogCharacter> It(World); It; ++It)
	{
		Save->DogLocation = It->GetActorLocation();
	}
	if (UFSPhoneSubsystem* Phone = GI->GetSubsystem<UFSPhoneSubsystem>())
	{
		Phone->GetState(Save->PhoneDelivered, Save->PhoneRead, Save->PhoneCalls, Save->PhoneExtras);
	}
	return UGameplayStatics::SaveGameToSlot(Save, SlotName(Index), 0);
}

bool UFSSaveGame::LoadSlot(const UObject* WorldContext, int32 Index)
{
	UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
	const UFSSaveGame* Save = Cast<UFSSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName(Index), 0));
	if (!World || !Save || Save->DataVersion != SaveVersion)
	{
		return false;
	}
	UGameInstance* GI = World->GetGameInstance();
	UFSMissionSubsystem* Mission = GI ? GI->GetSubsystem<UFSMissionSubsystem>() : nullptr;
	if (!Mission)
	{
		return false;
	}
	Mission->RestoreAcquired(Save->Acquired);
	Mission->SetClockMinutes(Save->ClockMinutes);
	if (AFSPrologueDirector* D = Cast<AFSPrologueDirector>(UGameplayStatics::GetActorOfClass(World, AFSPrologueDirector::StaticClass())))
	{
		D->ReadState(*Save);
	}
	if (AFSHeroCharacter* Hero = Cast<AFSHeroCharacter>(UGameplayStatics::GetPlayerPawn(World, 0)))
	{
		Hero->ReadState(*Save);
	}
	for (TActorIterator<AFSDogCharacter> It(World); It; ++It)
	{
		It->SetActorLocation(Save->DogLocation, false, nullptr, ETeleportType::TeleportPhysics);
		It->Recall();
	}
	if (UFSPhoneSubsystem* Phone = GI->GetSubsystem<UFSPhoneSubsystem>())
	{
		Phone->SetState(Save->PhoneDelivered, Save->PhoneRead, Save->PhoneCalls, Save->PhoneExtras);
	}
	return true;
}

FString UFSSaveGame::Describe(int32 Index)
{
	const UFSSaveGame* Save = UGameplayStatics::DoesSaveGameExist(SlotName(Index), 0)
		? Cast<UFSSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName(Index), 0)) : nullptr;
	if (!Save)
	{
		return FString();
	}
	if (Save->DataVersion != SaveVersion)
	{
		return TEXT("ancienne sauvegarde (incompatible)");
	}
	return FString::Printf(TEXT("%s — enregistré le %s"), *Save->Summary, *Save->SavedAt.ToString(TEXT("%d/%m à %H:%M")));
}
