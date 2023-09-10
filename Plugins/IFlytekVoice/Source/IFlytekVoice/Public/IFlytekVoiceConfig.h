#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

/**
 * 配置类，单例类
 */
class IFLYTEKVOICE_API FIFlytekVoiceConfig
{
public:
	FIFlytekVoiceConfig();
	~FIFlytekVoiceConfig();

	static FIFlytekVoiceConfig* Get();
	static void Destroy();

	void Init(const FString& InPath = FPaths::ProjectConfigDir() / TEXT("IFlytekConfig.ini"));
	void Save(const FString& InPath = FPaths::ProjectConfigDir() / TEXT("IFlytekConfig.ini"));

	FIFlytekUserInfo UserInfo;
	FIFlytekASRInfo ASRInfo;
	FIFlytekASDInfo ASDInfo;
	FIFlytekTTSInfo TTSInfo;
	FIFlytekTMInfo TMInfo;
	FIFlytekSDInfo SDInfo;

private:
	static FIFlytekVoiceConfig* Manage;
};
