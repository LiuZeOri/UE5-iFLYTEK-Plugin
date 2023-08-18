#include "IFlytekVoiceType.h"

FIFlytekUserInfo::FIFlytekUserInfo()
{
}

FIFlytekASRInfo::FIFlytekASRInfo()
{
	serverURL = TEXT("ws://rtasr.xfyun.cn/v1/ws");
	serverProtocol = TEXT("ws");
}

FString FIFlytekASRInfo::GetLanguageString() const
{
	switch (Language)
	{
	case EASRLanguageType::cn:
		return TEXT("cn");
	case EASRLanguageType::cn_cantonese:
		return TEXT("cn_cantonese");
	case EASRLanguageType::cn_henanese:
		return TEXT("cn_henanese");
	case EASRLanguageType::cn_xinanese:
		return TEXT("cn_xinanese");
	case EASRLanguageType::en:
		return TEXT("en");
	case EASRLanguageType::ar:
		return TEXT("ar");
	case EASRLanguageType::de:
		return TEXT("de");
	case EASRLanguageType::es:
		return TEXT("es");
	case EASRLanguageType::fr:
		return TEXT("fr");
	case EASRLanguageType::it:
		return TEXT("it");
	case EASRLanguageType::ko:
		return TEXT("ko");
	case EASRLanguageType::ru:
		return TEXT("ru");
	case EASRLanguageType::ja:
		return TEXT("ja");
	case EASRLanguageType::vi:
		return TEXT("vi");
	default:
		return TEXT("cn");
	}
}
