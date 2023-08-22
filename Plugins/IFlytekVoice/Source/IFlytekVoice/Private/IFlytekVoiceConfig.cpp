#include "IFlytekVoiceConfig.h"

FIFlytekVoiceConfig* FIFlytekVoiceConfig::Manage = nullptr;

FIFlytekVoiceConfig::FIFlytekVoiceConfig()
{
}

FIFlytekVoiceConfig::~FIFlytekVoiceConfig()
{
}

FIFlytekVoiceConfig* FIFlytekVoiceConfig::Get()
{
	if (!Manage)
	{
		Manage = new FIFlytekVoiceConfig();
	}
	return Manage;
}

void FIFlytekVoiceConfig::Destroy()
{
	if (Manage)
	{
		delete Manage;
	}

	Manage = nullptr;
}

void FIFlytekVoiceConfig::Init(const FString& InPath)
{
	//相对路径转化为全路径
	FString FullPath = FPaths::ConvertRelativePathToFull(InPath);
	if (GConfig)
	{
		/**
		* 以下是两个lambda表达式的匿名函数
		* @param Section 为配置标题，即[]中间的内容
		* @param Key 键，等号前面的内容
		* @param Value 值，等号后面的内容
		*/
		auto GetStringValue = [&](const TCHAR* Section, const TCHAR* Key, FString& Value)
		{
			if (!GConfig->GetString(Section, Key, Value, FullPath))
			{
				GConfig->SetString(Section, Key, *Value, FullPath);
				GConfig->Flush(false, FullPath);
			}
		};
		auto GetInt32Value = [&](const TCHAR* Section, const TCHAR* Key, int32& Value)
		{
			if (!GConfig->GetInt(Section, Key, Value, FullPath))
			{
				GConfig->SetInt(Section, Key, Value, FullPath);
				GConfig->Flush(false, FullPath);
			}
		};
		auto GetBoolValue = [&](const TCHAR* Section, const TCHAR* Key, bool& Value)
		{
			if (!GConfig->GetBool(Section, Key, Value, FullPath))
			{
				GConfig->SetBool(Section, Key, Value, FullPath);
				GConfig->Flush(false, FullPath);
			}
		};

		// [IFlytekUserConfig] 配置
		const TCHAR* IFlytekUserSection = TEXT("IFlytekUserConfig");
		GetStringValue(IFlytekUserSection, TEXT("appID"), UserInfo.appID);
		GetStringValue(IFlytekUserSection, TEXT("apiSecret"), UserInfo.apiSecret);
		GetStringValue(IFlytekUserSection, TEXT("apiKeyASR"), UserInfo.apiKeyASR);
		GetStringValue(IFlytekUserSection, TEXT("apiKeyTTS"), UserInfo.apiKeyTTS);

		// [IFlytekASRConfig] 配置
		const TCHAR* IFlytekASRSection = TEXT("IFlytekASRConfig");
		GetStringValue(IFlytekASRSection, TEXT("serverURL"), ASRInfo.serverURL);
		GetStringValue(IFlytekASRSection, TEXT("serverProtocol"), ASRInfo.serverProtocol);

		// [IFlytekTTSConfig] 配置
		const TCHAR* IFlytekTTSSection = TEXT("IFlytekTTSConfig");
		GetStringValue(IFlytekTTSSection, TEXT("serverURL"), TTSInfo.serverURL);
		GetStringValue(IFlytekTTSSection, TEXT("serverProtocol"), TTSInfo.serverProtocol);
		GetStringValue(IFlytekTTSSection, TEXT("host"), TTSInfo.host);
	}
}

void FIFlytekVoiceConfig::Save(const FString& InPath)
{
	FString FullPath = FPaths::ConvertRelativePathToFull(InPath);

	auto SetInt32Value = [&](const TCHAR* Section, const TCHAR* Key, int32 Value)
	{
		if (!GConfig->GetInt(Section, Key, Value, FullPath))
		{
			GConfig->SetInt(Section, Key, Value, FullPath);
			GConfig->Flush(false, FullPath);
		}
	};
}
