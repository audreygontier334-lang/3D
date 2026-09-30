#include "FSSettings.h"
#include "Misc/ConfigCacheIni.h"

namespace
{
	const TCHAR* Section = TEXT("FauxSemblants");

	float GetFloat(const TCHAR* Key, float Default)
	{
		float Value = Default;
		if (GConfig) { GConfig->GetFloat(Section, Key, Value, GGameUserSettingsIni); }
		return Value;
	}

	bool GetBool(const TCHAR* Key, bool bDefault)
	{
		bool bValue = bDefault;
		if (GConfig) { GConfig->GetBool(Section, Key, bValue, GGameUserSettingsIni); }
		return bValue;
	}

	void SetFloat(const TCHAR* Key, float Value)
	{
		if (!GConfig) { return; }
		GConfig->SetFloat(Section, Key, Value, GGameUserSettingsIni);
		GConfig->Flush(false, GGameUserSettingsIni);
	}

	void SetBool(const TCHAR* Key, bool bValue)
	{
		if (!GConfig) { return; }
		GConfig->SetBool(Section, Key, bValue, GGameUserSettingsIni);
		GConfig->Flush(false, GGameUserSettingsIni);
	}
}

float FSSettings::MouseSensitivity() { return FMath::Clamp(GetFloat(TEXT("MouseSensitivity"), 1.f), 0.25f, 3.f); }
void FSSettings::SetMouseSensitivity(float Value) { SetFloat(TEXT("MouseSensitivity"), FMath::Clamp(Value, 0.25f, 3.f)); }
bool FSSettings::InvertY() { return GetBool(TEXT("InvertY"), false); }
void FSSettings::SetInvertY(bool bValue) { SetBool(TEXT("InvertY"), bValue); }
float FSSettings::SubtitleScale() { return FMath::Clamp(GetFloat(TEXT("SubtitleScale"), 1.f), 1.f, 1.5f); }
void FSSettings::SetSubtitleScale(float Value) { SetFloat(TEXT("SubtitleScale"), FMath::Clamp(Value, 1.f, 1.5f)); }
bool FSSettings::ExtendedActionTime() { return GetBool(TEXT("ExtendedActionTime"), false); }
void FSSettings::SetExtendedActionTime(bool bValue) { SetBool(TEXT("ExtendedActionTime"), bValue); }

namespace
{
	const TCHAR* VolumeKey(FSSettings::EFSVolume Category)
	{
		switch (Category)
		{
		case FSSettings::EFSVolume::Music: return TEXT("VolumeMusic");
		case FSSettings::EFSVolume::Effects: return TEXT("VolumeEffects");
		case FSSettings::EFSVolume::Voice: return TEXT("VolumeVoice");
		case FSSettings::EFSVolume::Ambience: return TEXT("VolumeAmbience");
		case FSSettings::EFSVolume::Interface: return TEXT("VolumeInterface");
		default: return TEXT("VolumeMaster");
		}
	}
}

float FSSettings::Volume(EFSVolume Category) { return FMath::Clamp(GetFloat(VolumeKey(Category), Category == EFSVolume::Master ? 0.8f : 1.f), 0.f, 1.f); }
void FSSettings::SetVolume(EFSVolume Category, float Value) { SetFloat(VolumeKey(Category), FMath::Clamp(Value, 0.f, 1.f)); }
bool FSSettings::Muted() { return GetBool(TEXT("Muted"), false); }
void FSSettings::SetMuted(bool bValue) { SetBool(TEXT("Muted"), bValue); }
