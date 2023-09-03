#include "IFlytekVoiceType.h"

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

FIFlytekASDInfo::FIFlytekASDInfo()
{
	serverURL = TEXT("ws://iat-api.xfyun.cn/v2/iat");
	serverProtocol = TEXT("ws");
	host = TEXT("iat-api.xfyun.cn");
	format = EAudioFormat::rate16k;
	language = EASDLanguageType::zh_cn;
	bUseMinorLanguage = false;
	domain = EASDDomainType::iat;
	accent = TEXT("mandarin");
	bUseVAD = false;
	vad_eos = 2000;
	bUseDwa = false;
	bUsePersonalizationParameter = false;
	pd = EPersonalizationParameter::game;
	bPunctuation = true;
	typeFace = ETypeFace::zh_cn;
	bNunum = true;
};

FString FIFlytekASDInfo::GetLanguageTypeString() const
{
	switch (language)
	{
	case EASDLanguageType::zh_cn:
		return TEXT("zh_cn");
	case EASDLanguageType::en_us:
		return TEXT("en_us");
	default:
		return TEXT("zh_cn");
	}
}

FString FIFlytekASDInfo::GetDomainTypeString() const
{
	switch (domain)
	{
	case EASDDomainType::iat:
		return TEXT("iat");
	case EASDDomainType::medical:
		return TEXT("medical");
	case EASDDomainType::govSeatAssistant:
		return TEXT("gov-seat-assistant");
	case EASDDomainType::seatAssistant:
		return TEXT("seat-assistant");
	case EASDDomainType::govAnsys:
		return TEXT("gov-ansys");
	case EASDDomainType::govNav:
		return TEXT("gov-nav");
	case EASDDomainType::finNav:
		return TEXT("fin-nav");
	case EASDDomainType::finAnsys:
		return TEXT("fin-ansys");
	default:
		return TEXT("iat");
	}
}

FString FIFlytekASDInfo::GetPersonalizationParameterString() const
{
	switch (pd)
	{
	case EPersonalizationParameter::game:
		return TEXT("game");
	case EPersonalizationParameter::health:
		return TEXT("health");
	case EPersonalizationParameter::shopping:
		return TEXT("shopping");
	case EPersonalizationParameter::trip:
		return TEXT("trip");
	default:
		return TEXT("game");
	}
}

FString FIFlytekASDInfo::GetTypeFaceString() const
{
	switch (typeFace)
	{
	case ETypeFace::zh_cn:
		return TEXT("zh-cn");
	case ETypeFace::zh_hk:
		return TEXT("zh-hk");
	default:
		return TEXT("zh-cn");
	}
}

FString FIFlytekASDInfo::GetAudioFormatString() const
{
	switch (format)
	{
	case EAudioSampleRate::rate8k:
		return TEXT("audio/L16;rate=8000");
	case EAudioSampleRate::rate16k:
		return TEXT("audio/L16;rate=16000");
	default:
		return TEXT("audio/L16;rate=16000");
	}
}

FIFlytekTTSInfo::FIFlytekTTSInfo()
{
	serverURL = TEXT("ws://tts-api.xfyun.cn/v2/tts");
	serverProtocol = TEXT("ws");
	host = TEXT("tts-api.xfyun.cn");
	aue = EAudioEncodingType::raw;
	auf = EAudioSampleRate::rate16k;
	vcn = TEXT("xiaoyan");
	speed = 50;
	volume = 50;
	pitch = 50;
	bgs = false;
	reg = EEnglishPronunciationType::defaultMethod;
	rdn = ENumberPronunciationType::automaticJudgment;
}

FString FIFlytekTTSInfo::GetAudioEncodingTypeString() const
{
	switch (aue)
	{
	case EAudioEncodingType::raw:
		return TEXT("raw");
	case EAudioEncodingType::lame:
		return TEXT("lame");
	default:
		return TEXT("raw");
	}
}

FString FIFlytekTTSInfo::GetAudioSampleRateString() const
{
	switch (auf)
	{
	case EAudioSampleRate::rate8k:
		return TEXT("audio/L16;rate=8000");
	case EAudioSampleRate::rate16k:
		return TEXT("audio/L16;rate=16000");
	default:
		return TEXT("audio/L16;rate=16000");
	}
}

FString FIFlytekTTSInfo::GetEnglishPronunciationTypeString() const
{
	switch (reg)
	{
	case EEnglishPronunciationType::methodOne:
		return TEXT("0");
	case EEnglishPronunciationType::methodTwo:
		return TEXT("1");
	case EEnglishPronunciationType::methodThree:
		return TEXT("2");
	case EEnglishPronunciationType::defaultMethod:
		return TEXT("-1");
	default:
		return TEXT("-1");
	}
}

FString FIFlytekTTSInfo::GetNumberPronunciationTypeString() const
{
	switch (rdn)
	{
	case ENumberPronunciationType::automaticJudgment:
		return TEXT("0");
	case ENumberPronunciationType::fullNumericValue:
		return TEXT("1");
	case ENumberPronunciationType::fullString:
		return TEXT("2");
	case ENumberPronunciationType::stringFirst:
		return TEXT("3");
	default:
		return TEXT("0");
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

FTTSSocketResponded::FTTSSocketResponded()
{
	code = -1;
}

TArray<uint8> FTTSSocketResponded::GetAudioDecodeData() const
{
	TArray<uint8> audioDecodeData;
	FBase64::Decode(audio, audioDecodeData);
	
	return audioDecodeData;
}
