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
