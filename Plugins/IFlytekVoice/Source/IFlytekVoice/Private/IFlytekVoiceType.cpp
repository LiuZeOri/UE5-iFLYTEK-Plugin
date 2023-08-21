﻿#include "IFlytekVoiceType.h"

FIFlytekUserInfo::FIFlytekUserInfo()
{
}

FIFlytekASRInfo::FIFlytekASRInfo()
{
	serverURL = TEXT("ws://rtasr.xfyun.cn/v1/ws");
	serverProtocol = TEXT("ws");
	Language = EASRLanguageType::cn;
	bTranslation = false;
	translationStrategy = EASRTranslationStrategy::strategyTwo;
	translationLanguageType = EASRTranslationLanguageType::en;
	bFilterPunctuation = false;
	bUsePersonalizationParameter = false;
	PersonalizationParameters = EASRPersonalizationParameters::edu;
	bUseNearOrFarField = false;
	FieldType = EASRFieldType::farField;
	bUseRoleSeparation = false;
	languageRecognitionMode = ELanguageRecognitionMode::ChineseAndEnglishMode;
}

FString FIFlytekASRInfo::GetLanguageTypeString() const
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

FString FIFlytekASRInfo::GetTranslationStrategyString() const
{
	switch (translationStrategy)
	{
	case EASRTranslationStrategy::strategyOne:
		return TEXT("1");
	case EASRTranslationStrategy::strategyTwo:
		return TEXT("2");
	case EASRTranslationStrategy::strategyThree:
		return TEXT("3");
	default:
		return TEXT("2");
	}
}

FString FIFlytekASRInfo::GetTranslationLanguageTypeString() const
{
	switch (translationLanguageType)
	{
	case EASRTranslationLanguageType::cn:
		return TEXT("cn");
	case EASRTranslationLanguageType::cn_cantonese:
		return TEXT("cn_cantonese");
	case EASRTranslationLanguageType::en:
		return TEXT("en");
	case EASRTranslationLanguageType::ja:
		return TEXT("ja");
	case EASRTranslationLanguageType::ko:
		return TEXT("ko");
	case EASRTranslationLanguageType::ru:
		return TEXT("ru");
	case EASRTranslationLanguageType::fr:
		return TEXT("fr");
	case EASRTranslationLanguageType::es:
		return TEXT("es");
	case EASRTranslationLanguageType::vi:
		return TEXT("vi");
	default:
		return TEXT("en");
	}
}

FString FIFlytekASRInfo::GetPersonalizationParameterString() const
{
	switch (PersonalizationParameters)
	{
	case EASRPersonalizationParameters::court:
		return TEXT("court");
	case EASRPersonalizationParameters::edu:
		return TEXT("edu");
	case EASRPersonalizationParameters::finance:
		return TEXT("finance");
	case EASRPersonalizationParameters::medical:
		return TEXT("medical");
	case EASRPersonalizationParameters::tech:
		return TEXT("tech");
	case EASRPersonalizationParameters::isp:
		return TEXT("isp");
	case EASRPersonalizationParameters::gov:
		return TEXT("gov");
	case EASRPersonalizationParameters::ecom:
		return TEXT("ecom");
	case EASRPersonalizationParameters::mil:
		return TEXT("mil");
	case EASRPersonalizationParameters::com:
		return TEXT("com");
	case EASRPersonalizationParameters::life:
		return TEXT("life");
	case EASRPersonalizationParameters::car:
		return TEXT("car");
	default:
		return TEXT("edu");
	}
}

FString FIFlytekASRInfo::GetFieldType() const
{
	switch (FieldType)
	{
	case EASRFieldType::nearField:
		return TEXT("2");
	case EASRFieldType::farField:
		return TEXT("1");
	default:
		return TEXT("1");
	}
}

FString FIFlytekASRInfo::GetLanguageRecognitionMode() const
{
	switch (languageRecognitionMode)
	{
	case ELanguageRecognitionMode::ChineseAndEnglishMode:
		return TEXT("1");
	case ELanguageRecognitionMode::ChineseAndLessEnglishMode:
		return TEXT("2");
	case ELanguageRecognitionMode::PureChineseMode:
		return TEXT("4");
	default:
		return TEXT("1");
	}
}

FRecordingConfig::FRecordingConfig()
{
	SampleRate = 16000;
	Channels = 1;
	BitsPerSample = 16;
	BufferSize = 1024 * 10;
	ExtractDataTime = 40;
}

FASRSocketResponded::FASRSocketResponded()
{
	isEnd = false;
	type = -1;
}
