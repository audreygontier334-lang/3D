#include "FSKeyBindings.h"
#include "GameFramework/InputSettings.h"

namespace
{
	bool IsKeyboardOrMouse(const FKey& Key)
	{
		return Key.IsValid() && Key != EKeys::Escape && !Key.IsGamepadKey() && !Key.IsAxis1D() && !Key.IsAxis2D() && !Key.IsTouch();
	}

	TArray<FKey> CurrentKeys(const FFSBinding& B)
	{
		TArray<FKey> Keys;
		UInputSettings* S = UInputSettings::GetInputSettings();
		if (B.AxisScale == 0.f)
		{
			TArray<FInputActionKeyMapping> Maps;
			S->GetActionMappingByName(FName(B.Name), Maps);
			for (const FInputActionKeyMapping& M : Maps) { if (IsKeyboardOrMouse(M.Key)) { Keys.Add(M.Key); } }
		}
		else
		{
			TArray<FInputAxisKeyMapping> Maps;
			S->GetAxisMappingByName(FName(B.Name), Maps);
			for (const FInputAxisKeyMapping& M : Maps)
			{
				if (IsKeyboardOrMouse(M.Key) && FMath::Sign(M.Scale) == FMath::Sign(B.AxisScale)) { Keys.Add(M.Key); }
			}
		}
		return Keys;
	}

	void SetKeys(const FFSBinding& B, const TArray<FKey>& Keys)
	{
		UInputSettings* S = UInputSettings::GetInputSettings();
		if (B.AxisScale == 0.f)
		{
			TArray<FInputActionKeyMapping> Maps;
			S->GetActionMappingByName(FName(B.Name), Maps);
			for (const FInputActionKeyMapping& M : Maps) { if (IsKeyboardOrMouse(M.Key)) { S->RemoveActionMapping(M, false); } }
			for (const FKey& K : Keys) { S->AddActionMapping(FInputActionKeyMapping(FName(B.Name), K), false); }
		}
		else
		{
			TArray<FInputAxisKeyMapping> Maps;
			S->GetAxisMappingByName(FName(B.Name), Maps);
			for (const FInputAxisKeyMapping& M : Maps)
			{
				if (IsKeyboardOrMouse(M.Key) && FMath::Sign(M.Scale) == FMath::Sign(B.AxisScale)) { S->RemoveAxisMapping(M, false); }
			}
			for (const FKey& K : Keys) { S->AddAxisMapping(FInputAxisKeyMapping(FName(B.Name), K, B.AxisScale), false); }
		}
	}

	void Commit()
	{
		UInputSettings* S = UInputSettings::GetInputSettings();
		S->SaveKeyMappings();
		S->ForceRebuildKeymaps();
	}
}

const TArray<FFSBinding>& FSKeyBindings::All()
{
	static const TArray<FFSBinding> List = {
		{ TEXT("Avancer"), TEXT("MoveForward"), 1.f, { EKeys::Z, EKeys::W } },
		{ TEXT("Reculer"), TEXT("MoveForward"), -1.f, { EKeys::S } },
		{ TEXT("Aller à gauche"), TEXT("MoveRight"), -1.f, { EKeys::Q, EKeys::A } },
		{ TEXT("Aller à droite"), TEXT("MoveRight"), 1.f, { EKeys::D } },
		{ TEXT("Courir"), TEXT("Run"), 0.f, { EKeys::LeftShift } },
		{ TEXT("Parler, « Ariane, vas-y ! »"), TEXT("ActEnvoyer"), 0.f, { EKeys::E } },
		{ TEXT("Photographier"), TEXT("ActPhoto"), 0.f, { EKeys::F } },
		{ TEXT("Crier « Lila ! »"), TEXT("ActCrier"), 0.f, { EKeys::C } },
		{ TEXT("Ariane : au pied"), TEXT("Rappel"), 0.f, { EKeys::R } },
		{ TEXT("Ariane : reste"), TEXT("Reste"), 0.f, { EKeys::X } },
		{ TEXT("Ariane : cherche"), TEXT("Cherche"), 0.f, { EKeys::G } },
		{ TEXT("Ariane : montre"), TEXT("Montre"), 0.f, { EKeys::M } },
		{ TEXT("Ariane : « Vas-y ! » (là où je regarde)"), TEXT("VaLaBas"), 0.f, { EKeys::K } },
		{ TEXT("Ariane : « Non ! »"), TEXT("Non"), 0.f, { EKeys::N } },
		{ TEXT("Ariane : « C'est bien ! »"), TEXT("Bravo"), 0.f, { EKeys::B } },
		{ TEXT("Appeler le 17"), TEXT("Appel17"), 0.f, { EKeys::T } },
		{ TEXT("Vue épaule"), TEXT("CameraShoulder"), 0.f, { EKeys::One } },
		{ TEXT("Vue reculée"), TEXT("CameraWide"), 0.f, { EKeys::Two } },
		{ TEXT("Vue subjective"), TEXT("CameraFirst"), 0.f, { EKeys::Three } },
		{ TEXT("Changer de vue"), TEXT("CameraCycle"), 0.f, { EKeys::V } },
		{ TEXT("Changer d'épaule"), TEXT("SwapShoulder"), 0.f, { EKeys::Tab } },
		{ TEXT("Carnet"), TEXT("Carnet"), 0.f, { EKeys::J } },
		{ TEXT("Inventaire"), TEXT("Inventaire"), 0.f, { EKeys::I } },
		{ TEXT("Lampe torche"), TEXT("Lampe"), 0.f, { EKeys::L } },
		{ TEXT("Sauvegarde rapide"), TEXT("Save"), 0.f, { EKeys::F5 } },
		{ TEXT("Chargement rapide"), TEXT("Load"), 0.f, { EKeys::F9 } },
		{ TEXT("Menu principal"), TEXT("Pause"), 0.f, { EKeys::P } },
	};
	return List;
}

FString FSKeyBindings::KeysText(const FFSBinding& Binding)
{
	TArray<FString> Names;
	for (const FKey& K : CurrentKeys(Binding)) { Names.Add(K.GetDisplayName().ToString()); }
	return Names.Num() ? FString::Join(Names, TEXT(" / ")) : FString(TEXT("—"));
}

bool FSKeyBindings::IsAssignable(const FKey& Key)
{
	return IsKeyboardOrMouse(Key) && Key != EKeys::Escape;
}

FString FSKeyBindings::Rebind(const FFSBinding& Binding, const FKey& NewKey)
{
	if (!IsAssignable(NewKey))
	{
		return FString();
	}
	// Si la touche servait déjà à une autre commande, celle-ci récupère l'ancienne touche (échange).
	const TArray<FKey> Old = CurrentKeys(Binding);
	FString Swapped;
	for (const FFSBinding& Other : All())
	{
		if (&Other == &Binding) { continue; }
		TArray<FKey> OtherKeys = CurrentKeys(Other);
		if (OtherKeys.Remove(NewKey) > 0)
		{
			if (Old.Num() > 0 && !OtherKeys.Contains(Old[0])) { OtherKeys.Add(Old[0]); }
			SetKeys(Other, OtherKeys);
			Swapped = Other.Label;
		}
	}
	SetKeys(Binding, { NewKey });
	Commit();
	return Swapped;
}

void FSKeyBindings::ResetDefaults()
{
	for (const FFSBinding& B : All())
	{
		SetKeys(B, B.Defaults);
	}
	Commit();
}
